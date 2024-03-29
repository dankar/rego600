# esphome based Rego 600 controller/supervisor

## Read this part first, you have to!

**I make absolutely no guarantees and take no responsibility for anything done by or with this code.** I wrote this and connected to my own Rego 600, and it still works, but I have no idea what it might do to yours. It's a very expensive piece of equipment that you might destroy by touching the wrong part.

**Take care when connecting anything electrically to your heat pump.** The pump contains three phase power and who knows how sensitive the controller ICs are to electrostatic discharge. I just used a level converter between the serial interface and a nodemcu, but you should use opto-couplers, isolated power supplies, safety carabiners and probably at least three condoms.

**This code is written specifically for the Rego 600.** Not the 636, not the 71042 or whatever. There are documents specifying the registers for other versions so this code can probably be adapted to other models, but you can't and shouldn't use this for anything other than model 600. **If you use this code on any other model, the code might instruct it to detonate, who knows.** The code has no idea what model it is talking to. It might be communicating with a space shuttle or a banana for all it cares.

## What it does

Here's a view in home assistant of the data and controls provided by the code:

![home assistant](hardware/hass.jpeg)

## Hardware

You can connect almost any microcontroller directly to the heat pump serial interface. The only caveat is that the heat pump serial interface runs on 5 volts and your microcontroller might be 3.3 volts. In that case you must convert the voltages on the serial interface between them. I used a simple [level converter](https://www.sparkfun.com/products/12009) to do the job, but you might want to consider creating a better circuit using opto-couplers.

If you want to use the configuration as it is, you should connect RX on the Rego 600 (pin 2 on the DB9) to TX on the microcontroller (D0 in this case) and TX on the Rego (pin 3 on the DB9) to RX on the microcontroller (D2), with a level converter inbetween. The DB9 on the Rego has 5 volts out (pin 4), which can be used to drive the high side of the level converter.

The following is a schematic of what I use:

![schematic](hardware/schematic.png)

## Building and deploying

If you already have some experience with esphome you might already know what's up. Check out the ```heat_pump.yaml``` and modify it for your use case. You should be able to use any microcontroller that supports esphome and can do UART serial. Change pins if you want to.

If you don't know about esphome you probably have to install it first.

After that you need to create a file called ```secrets.yaml``` in the same folder as the code and add the following to it:

```
wifi_ssid: "Your wifi ssid here"
wifi_password: "Your wifi password here"
```

You might also want to change the "Fallback hotspot" password in the ```heat_pump.yaml``` file.

Unless modified, the ```heat_pump.yaml``` specifies that you use a nodemcuv2 microcontroller, with TX of the UART connected to D0 and RX connected to D2. The inversion is also specified for the circuit with a level converter. If you constructed the hardware in any other way, then this would need to be changed.

Then you should be able to connect your microcontroller and compile/install with the following command:

```
python3 -m esphome run heat_pump.yaml
```

After you've done this once, the same command can also update the firmware on the microcontroller via wifi.

The device should be reachable on your LAN, with the adress http://heatpump/ unless you changed the device name. The API is also enabled so it should be possible to add it to e.g. home assistant with the same name.

## Home assistant setup for controlling display

The display data can be read out from the heat pump controller. Currently only latin9 is supported, [modify the code](rego600_display.cpp) if you want other encodings. Home Assistant expects UTF-8.

**Warning**! There seems to be a bug in the Rego600 controller that causes a lot of timeouts when reading values after entering the "Info" screen from the default view on the display. I've tried to handle this as best I can in the code, but if this screen is entered, a lot of values will fail to be read, including the screen. It can be fixed by exiting the menu (manually, or by blindly pressing the buttons in Home Assistant) or waiting for the screen to timeout and go back to the default screen. I'm still investigating this.

Another curiosity is that there is no known way of instructing the controller to hold down a button, which is needed for the higher user levels in the menus.

Here is an example of a display and controls setup in Home Assistant:

![image of display and controls](hardware/display.png)

And here is the card setup in Home Assistant that produces the above picture:

```yaml
type: vertical-stack
cards:
  - type: custom:html-template-card
    ignore_line_breaks: true
    content: >
      <p style="font-size: 1.5em;white-space: break-spaces;font-family:'Lucida
      Console', monospace"> {{ states('sensor.display_line_1') }}</p> <p
      style="font-size: 1.5em;white-space: break-spaces;font-family:'Lucida
      Console', monospace"> {{ states('sensor.display_line_2') }}</p> <p
      style="font-size: 1.5em;white-space: break-spaces;font-family:'Lucida
      Console', monospace"> {{ states('sensor.display_line_3') }}</p> <p
      style="font-size: 1.5em;white-space: break-spaces;font-family:'Lucida
      Console', monospace"> {{ states('sensor.display_line_4') }}</p>
  - type: horizontal-stack
    cards:
      - show_name: false
        show_icon: true
        type: button
        tap_action:
          action: toggle
        entity: button.key_1
      - show_name: false
        show_icon: true
        type: button
        tap_action:
          action: toggle
        entity: button.key_2
      - show_name: false
        show_icon: true
        type: button
        tap_action:
          action: toggle
        entity: button.key_3
  - type: horizontal-stack
    cards:
      - show_name: false
        show_icon: true
        type: button
        tap_action:
          action: toggle
        entity: button.wheel_left
        icon: mdi:arrow-collapse-left
      - show_name: false
        show_icon: true
        type: button
        tap_action:
          action: toggle
        entity: button.wheel_right
        icon: mdi:arrow-collapse-right
```

## Further development

There is, as far as I know, no official documentation of this protocol. I've implemented most of the registers that I could find in different sources online (one part that is currently missing from the code, but is fairly well documented, is reading out alarm logs, as well as reading the lines on the display).

Apart from possibly missing registers, there are some other open questions:
* Minimum and maximum limits of different settings.
* It might be possible to write to some of the registers which this code is currently just reading from.
* General undocumented stuff.

The code is also in dire need of a big cleanup, which might happen any day now.
