# Scoop Duty
"Scoop duty" is a small art piece that indicates whose turn it is to scoop kitty litter, based on the current time.  It uses an [Arduino Nano](https://store-usa.arduino.cc/collections/boards/products/arduino-nano) control the LEDs and handle external input.

TODO: add demo gif

## Behavior
The Arduino Nano can only keep track of how much time has passed since it powered on, so it needs to be told what time it is upon startup.

After the time is configured, the turn indicator lights will change daily, at midnight.  The external button can be used if the turn needs to be manually changed for any reason (e.g., both of us are out of town for an odd number of days).

```mermaid
stateDiagram-v2
note left of StartupConfig
NEXT = Long button press (high beep)
INCREMENT = Short button press (low beep)
TIMER = Runs daily at midnight (no beep)
end note
state StartupConfig {
    direction LR
    state "Set hour\n(left blinking)" as HourSetup
    state "Set minute\n(right blinking)" as MinuteSetup
    [*] --> HourSetup
    HourSetup --> HourSetup: INCREMENT
    HourSetup --> MinuteSetup: NEXT
    MinuteSetup --> MinuteSetup: INCREMENT
    MinuteSetup --> [*]: NEXT
}
state Main {
    direction LR
    state "Tanner's turn\n(left light)" as Left
    state "Ali's turn\n(right light)" as Right
    [*] --> Left
    Left --> Right: INCREMENT
    Left --> Right: TIMER
    Right --> Left: INCREMENT
    Right --> Left: TIMER
}
[*] --> StartupConfig: Power on
StartupConfig --> Main
StartupConfig --> [*]: Power off
Main --> [*]: Power off
```

## Schematic
```mermaid
flowchart LR
Pin2 ---|+| Piezo((Piezo)) ---|-| GND
Pin3-6,8-11 ---|+| LED((LED)) ---|-| R1{{330Ω}} --- GND
Pin12 --- Button((Button)) --- GND
```

### Legend
| Shape | Description |
| - | - |
| Rectangle | Arduino Pin |
| Line | Wire |
| Circle | IO element |
| Hexagon | Resistor |
