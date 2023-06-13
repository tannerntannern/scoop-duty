# Scoop Duty
"Scoop duty" is a small art piece that indicates whose turn it is to scoop kitty litter.  Red light means it's my day to scoop, and yellow light means it's my partner's turn to scoop.  Every day the lights automatically switch at midnight.

## Finished product
[Progress pics](#progress-pics) are at the end of this document

![](./img/done1.jpg)
![](./img/done2.jpg)
![](./img/done3.jpg)

While the lights automatically change, the button can be used if the turn needs to be manually changed for any reason (e.g., we're out of town and order gets messed up).

![](./img/demo.gif)

## Motivation
Aside from just being a fun art piece, this device has actual, practical utility.  It's surpisingly easy to lose track of who's on scoop duty, especially since our morning routines don't overlap (and that's when we scoop).

The embarassing frequency at which we both "double scooped" or forgot to scoop (more commonly the former) was the inspiration for the project.  It's an over-engineered solution to this trivial problem.

## Behavior
The Arduino Nano can only keep track of how much time has passed since it powered on, so it needs to be told what time it is upon startup (in order to know when midnight is). After the time is configured, the turn indicator lights will change daily, at midnight.

```mermaid
stateDiagram-v2
note left of StartupConfig
NEXT = Long button press (beep jingle)
INCREMENT = Short button press (low beep)
TIMER = Runs daily at midnight (no beep)
end note
state StartupConfig {
    direction LR
    state "Set hour\n(left light)" as HourSetup
    state "Set minute\n(right light)" as MinuteSetup
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

# Make your own
This repository has everything you need to make your own "scoop duty" frame if you'd like!

We used an [Arduino Nano](https://store-usa.arduino.cc/products/arduino-nano?selectedStore=us) but any Arduino board should work.  Aside from that, the only special components you'll need are LEDs, a Piezo (beeper), and some 330Ω resistors (one for each LED), and a button switch.

How you build the rest is up to you, but this is what we used:
- shadow box frame from a local arts & crafts store along with
- veneer to carve the cat outlines into
- parchment paper to diffuse the LED lighting
- a chunk of wood from the frame itself to divide the inside of the box and prevent the light from bleeding to the opposite side
- lots of hot glue and solder to put it all together

## Source Code
You're welcome to use the source code for this project in any way you'd like: [scoop_duty/scoop_duty.ino](./scoop_duty/scoop_duty.ino).  If you're not keen on programming, this will probably save you some time.

The code assumes the following schematic, but it can easily be adapted if you wire up your project differently.

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

# Progress pics
Initial sketch

![](./img/prog1.jpg)

Breadboard prototype

![](./img/prog2.jpg)

Button and (custom) power socket inserted into the frame

![](./img/prog3.jpg)
![](./img/prog4.jpg)

Rough layout of the LEDs and initial soldering

![](./img/prog5.jpg)

Wiring it all together

![](./img/prog6.jpg)
![](./img/prog7.jpg)

Cat tax

![Earl Grey](./img/earl.jpg)
