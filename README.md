# tec-Laser-Raster-Display
Laser Raster mag-mech-xy

![image](https://github.com/user-attachments/assets/b0f3ac7f-1304-4a97-ba2a-30200f4ecdf7)
![image](https://github.com/user-attachments/assets/f5f0371c-148f-4980-835c-1c44e7224f84)
![image](https://github.com/user-attachments/assets/3b032671-82bd-452b-9086-8983af55e912)


![image](https://github.com/user-attachments/assets/e310d747-31a6-493b-8d63-0257bfe31b64)


# TEC-1 Laser Display System: Technical Overview

- The system combines precise digital control with mechanical scanning to create visible patterns.
- Success depends on careful calibration and timing adjustments for the specific hardware setup.



## Hardware Components

### 1. Mirror Deflection System
- **Voice Coil Actuators**
  - Two voice coils (similar to speaker coils)
  - Each coil controls one axis (X or Y)
  - Spring mounting provides return force
  - Attached small mirrors reflect laser beam
  - Typical deflection range: ±10 degrees

- **DAC (Digital to Analog Converter)**
  - Two 8-bit DACs (0-255 steps)
  - Output range: 0-5V
  - Port #E0: X-axis control
  - Port #E1: Y-axis control
  - Current amplifiers drive voice coils
  - Settling time ~1ms per position

### 2. Laser Module
- **Components**
  - Low-power laser diode (typically <5mW)
  - TTL control input
  - Collimating lens
  - Port #E2: ON/OFF control
  - Fast switching (<1µs)

### 3. Physical Setup
```
[Laser] --> [Mirror-Y] --> [Mirror-X] --> [Screen]
            (Up/Down)     (Left/Right)

Power Distribution:
+5V --> Laser Module
+12V -> Voice Coil Amplifiers
GND --> Common Ground
```

## Software Operation

### 1. Position Control System
```mint
:P            // Position Function
  y /O        // Y-DAC output
  x /O        // X-DAC output
  d()         // Mirror settling delay
;
```
- DAC values (0-255) control mirror angles
- Center position = 128 (2.5V)
- Settling delay prevents mirror oscillation

### 2. Drawing Methods

#### Point Drawing
```mint
:D            // Draw Dot
  P           // Position mirrors
  1 L         // Laser ON
  d()         // Hold time
  0 L         // Laser OFF
;
```

#### Vector Drawing
```
Start Point --> Laser ON --> Move Mirrors --> Laser OFF
     |              |            |              |
  DAC Set       Port #E2     DAC Ramp      Port #E2
```

#### Raster Scanning
```
Y-increment
    ^
    |     Scan Lines
    | →→→→→→→→→→→
    | ←←←←←←←←←←←
    | →→→→→→→→→→→
    Start
```

## Drawing Patterns

### 1. Simple Shapes

#### Box Drawing
```mint
:B            // Box Function
  " P         // Position start
  " H         // Top line
  " $ V       // Right side
  $ H         // Bottom
  V           // Left side
;
```
- Uses sequential line drawing
- Corner points defined by DAC values
- Lines drawn with laser continuously ON

### 2. Text Display

#### Character Formation
```
5x7 Matrix Example ('A'):
 ███  = 0 1 1 1 0
█   █ = 1 0 0 0 1
█   █ = 1 0 0 0 1
█████ = 1 1 1 1 1
█   █ = 1 0 0 0 1
```
- Each character defined in 5x7 bitmap
- Raster scanned point by point
- Spacing controlled by timing

## Timing Considerations

### 1. Critical Timings
```
Mirror Movement:   ~1ms
Laser Switch:     <1µs
Point Duration:   ~100µs
Frame Refresh:    ~20ms
```

### 2. Software Delays
```mint
d()         // Settling delay (~1ms)
1000()      // Frame delay (~20ms)
```

## Safety Features

### 1. Hardware Safety
- Laser power limiting
- Mechanical stops on mirrors
- Current limiting on coils
- Thermal protection

### 2. Software Safety
```mint
// Position Limits
:A            // Limit Check
  $ < (       // If over max
    '         // Drop value
    "         // Use max
  )
;
```

## Calibration Process

### 1. Mirror Alignment
```mint
:X            // X Calibration
  0 x /O      // Min position
  1000()      // Check
  m x /O      // Max position
  1000()      // Check
;
```

### 2. DAC Calibration
- Center position (128) = 0° deflection
- Full range calibration
- Linearity check
- Cross-axis compensation

## System Integration

### 1. TEC-1 Interface
```
Z80 CPU --> Port Control --> DAC --> Voice Coils
                        --> Laser Control
```

### 2. Memory Usage
- Character patterns in RAM
- Display buffer for text
- Position lookup tables

## Performance Factors

### 1. Resolution Limited By
- DAC precision (8 bits = 256 steps)
- Mirror mechanical response
- Laser spot size
- Scanning speed

### 2. Speed Limited By
- Mirror settling time
- Z80 execution speed
- DAC update rate
- Mechanical resonance

## Operating Parameters

### 1. Typical Settings
```mint
255 m!  // Maximum DAC value
128 c!  // Center position
20 s!   // Scan step size
5 d!    // Dot duration
```

### 2. Adjustable Factors
- Scanning density
- Pattern size
- Refresh rate
- Brightness (duty cycle)



# TEC-1 Laser Display Control System - DAC Implementation

- provides basic building blocks for more complex patterns and text display.
- All timing values should be calibrated for the specific hardware setup.

## Port Definitions
```mint
// DAC control ports
#E0 x!  // X-axis DAC port for mirror deflection
#E1 y!  // Y-axis DAC port for mirror deflection
#E2 z!  // Laser ON/OFF control port

// DAC value limits (0-255 for 8-bit DAC)
255 m!  // Maximum DAC value
128 c!  // Center position

// Scanning limits
20 s!   // Scan step size
5 d!    // Dot duration
```

## Core Mirror Control Functions
```mint
// Function P: Position Mirror
:P            // Takes x y positions
  y /O        // Set Y-axis DAC
  x /O        // Set X-axis DAC
  d()         // Settling delay for mirror
;

// Function L: Laser Control
:L            // Takes state (0/1)
  z /O        // Output to laser port
;

// Function D: Draw Dot
:D            // Takes x y coordinates
  P           // Position mirror
  1 L         // Laser on
  d()         // Hold time
  0 L         // Laser off
;
```

## Basic Pattern Functions
```mint
// Function H: Draw Horizontal Line
:H            // Takes length position
  " (         // Loop length times
    " /i P    // Position at current point
    1 L       // Laser on
  )
  0 L         // Laser off
;

// Function V: Draw Vertical Line
:V            // Takes length position
  " (         // Loop length times
    /i $ P    // Position at current point
    1 L       // Laser on
  )
  0 L         // Laser off
;

// Function B: Draw Box
:B            // Takes width height start-x start-y
  " P         // Position to start
  " H         // Top line
  " $ V       // Right side
  $ H         // Bottom line
  V           // Left side
;
```

## Scanning Patterns
```mint
// Function R: Raster Scan
:R            // Takes steps
  " (         // Outer loop (Y)
    " (       // Inner loop (X)
      /i /j P // Position using loop counters
      1 L     // Laser on
      d()     // Duration
      0 L     // Laser off
    )
  )
;

// Function S: Spiral Scan
:S            // Takes radius
  0 a!        // Start angle
  " (         // Loop radius times
    a         // Get current angle
    $ #57 * c + " P  // Convert to x position
    #57 * c + P      // Convert to y position
    1 L              // Laser on
    d()              // Duration
    0 L              // Laser off
    a 10 + a!        // Increment angle
  )
;
```

## Character Drawing
```mint
// Define 5x7 character patterns
[ 
  #1F #11 #11 #1F #11  // 'A'
] t!

// Function C: Draw Character
:C            // Takes char-pattern x y
  7 (         // 7 rows
    5 (       // 5 columns
      " /i $  // Get current position
      /j + P  // Add offsets
      t /i 5 * /j + ? // Get bit from pattern
      1 = (   // If bit is set
        1 L d() 0 L   // Draw dot
      )
    )
  )
;
```

## Test and Demo Functions
```mint
// Function T: Test Pattern
:T
  // Center position test
  c c P 1 L d() 0 L
  
  // Box test
  50 50 100 100 B
  
  // Raster test
  10 R
  
  // Spiral test
  50 S
;

// Function M: Main Loop
:M
  0 L         // Ensure laser starts off
  c c P       // Center mirrors
  /U (        // Infinite loop
    T         // Run test pattern
    1000()    // Delay between tests
    /K #51 = (// Check for exit key ('3')
      /T      // Break if pressed
    )
  )
;
```

## Calibration Functions
```mint
// Function X: X-axis Calibration
:X
  0 x /O      // Minimum position
  1000()      // Hold
  m x /O      // Maximum position
  1000()      // Hold
  c x /O      // Center position
;

// Function Y: Y-axis Calibration
:Y
  0 y /O      // Minimum position
  1000()      // Hold
  m y /O      // Maximum position
  1000()      // Hold
  c y /O      // Center position
;
```

## Usage Instructions

1. Basic Operation:
```mint
M             // Start main demo
```

2. Calibration:
```mint
X             // X-axis calibration
Y             // Y-axis calibration
```

3. Individual Tests:
```mint
50 50 100 100 B  // Draw 100x100 box at (50,50)
10 R             // Do 10-step raster scan
50 S             // Draw spiral with radius 50
```

## Hardware Notes

1. DAC Requirements:
   - 8-bit resolution minimum
   - 0-5V output range
   - Fast settling time (<1ms)

2. Mirror System:
   - Voice coil actuators
   - Adequate deflection range
   - Good frequency response
   - Proper damping

3. Laser Module:
   - TTL control input
   - Fast on/off switching
   - Proper power rating
   - Safety features

## Safety Considerations

1. Always ensure:
   - Laser starts in OFF state
   - Position limits are enforced
   - Emergency stop works
   - Mirror movement is damped

2. Physical Setup:
   - Secure mounting
   - Proper alignment
   - Heat dissipation
   - Beam containment
  
# ref

https://hackaday.com/2024/09/18/upgraded-raster-laser-projector-goes-rgb/

![image](https://github.com/user-attachments/assets/5efb9759-01ce-4228-9319-d4888851cbeb)



