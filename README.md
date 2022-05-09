# imageInversion_soc_microblaze
A DMA system that interfaces with an IP that inverts the image pixels received on an AXI4 stream interface. A microblaze processor writes the pixel data to a bram memory. DMA transfers them from memory to the IP and transfers the results back to the memory. The microblaze reads the results back from memory.
