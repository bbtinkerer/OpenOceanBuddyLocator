# Open Ocean Buddy Locator

By bbtinkerer (<http://bb-tinkerer.blogspot.com/>)

## Description

Device to give the direction of your buddy in the open ocean for diving, fishing, kayaking, etc. Know the location of your buddy without having to radio him in and give you directions.

Uses the Sony Spresense along with an Arduino Nano. The Spresense takes care of GPS and controlling the radio and OLED display and communicates to the Nano serially. The Nano controls the NeoPixel ring and magetometer.

Project instruction on Hackster.io (<https://www.hackster.io/bbtinkerer/open-ocean-buddy-locator-a582d8>)

Overview Video (<https://youtu.be/wy4cD6D_Nqs>)

Land Test Video (<https://youtu.be/Mx_nB5Qt_lc>)

Water Test Video (<https://youtu.be/INHeaZvJSXw>)

## Dependencies
Adafruit LSM303 library (<https://www.adafruit.com/product/1120>)

Adafruit NeoPixel libraries (<https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels>)

Adafruit OLED libraries (<https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples>)

RF24 (modified) - Original by TMRH20 (<http://tmrh20.github.io/RF24/>)

## Configuration

Use the Arudino IDE to upload the code to the Spresense and the Nano.

To get the Adafruit SSD1306 drivers to compile when the Spresense board is selected in the Arduino IDE, you will need to comment out a few blocks in the Adafruit_SSD1306.cpp. Comment out the blocks HAVE_PORTREG in the begin function. Also, remove or rename the files Adafruit_SPITFT.cpp, Adafruit_SPITFT.h and Adafruit_SPITFT_Macros.h in the Adafruit_GFX_Library.

### Known Issues

If you discover any bugs, feel free to create an issue on GitHub fork and
send a pull request.


## Authors

* bbtinkerer (https://github.com/bbtinkerer/)


## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request


## License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
