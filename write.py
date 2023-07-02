from itertools import cycle
import time
# Open the file for reading


with open('input.txt', 'r') as file:
    lines = file.readlines()  # Read all lines into a list
    line_cycle = cycle(lines)  # Create a cyclic iterator

    # Iterate indefinitely
    while True:
        try:
            line = next(line_cycle)  # Get the next line
            # print(line)  # Do something with the line
            file_write = open('/tmp/in.txt', 'w')
            file_write.write(line)
            file_write.close()
            time.sleep(0.5)
        except StopIteration:
            line_cycle = cycle(lines)  # Restart the cycle if end of file is reached
