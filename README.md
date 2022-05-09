# imageInversion_soc_microblaze
A DMA system that interfaces with an IP that invertes the image pixels received on an AXI4 stream interface. A microblaze processor writes the pixel data to a bram memory. DMA tranfers them from memory to the IP and tranfers the results back to the memory. The microblaze reads the results from memory.
