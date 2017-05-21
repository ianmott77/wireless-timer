# wireless-timer
A wireless timing system aimed at use for alpine ski racing. The system uses a combination of Raspberry Pi's and Arduino's


THIS IS NOT COMPLETE!!

Curently what is uploaded is a method of communicating between a Raspberry Pi and Arduino using I2C connections. The puprose of this library is to make communcation and sending of differnet data-tyes simple. The library creates a Packet object which is converted to JSON and then sent across the connection and decoded on the other side. The advantage of this is that no bit shifting is required to send data-types that take more than one byte. The other advantage of this library is that it takes care of splitting messages up into proper sized segments in order to fit into the 32 byte I2C buffer. The Arduino code is leak free and is capable of accepting long strings as a variable. You can find examples of how this library works in the examples folder. I would like to reiterate that the connection library is not finished yet, and is pre-alpha realistically.
