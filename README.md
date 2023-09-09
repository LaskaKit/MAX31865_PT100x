![TOP strana modulu](https://github.com/LaskaKit/MAX31865_PT100x/blob/main/img/2.jpg)

# LaskaKit MAX31865 converter for PT100 and PT1000

Converter module for PT100 and PT1000 platinum temperature sensors with SPI interface. Thanks to the MAX31865 chip, highly accurate measurement results are achieved for both four-wire and three-wire PTC sensors. The converter also supports two-wire sensors. However, for these, the accuracy cannot be expected to be as high as for multi-wire sensors. Communication with the microcontroller is via the SPI interface and the module has an integrated logic level translator so the module can be operated with 5V power and 3.3V logic, or vice versa.

The module is supplied in a configuration for the use of PTC sensors type PT100. If the module is to be used with a PT1000 type sensor, jumpers JP1 and JP3 must be cut and connected to the other side.

Setting the number of wires:</br>
4-wire sensors: JP2 and JP5 disconnected, JP4 connected at the top (default)</br>
3 wire sensors: JP2 disconnected, JP5 connected, JP4 connected down</br>
2 wire sensors: JP2 and JP5 connected, JP4 connected on top</br>

## The module is available on https://www.laskakit.cz/laskakit-max31865-prevodnik-pro-termoclanek-pt100-1000/
