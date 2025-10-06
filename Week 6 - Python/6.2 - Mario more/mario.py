import cs50

# Prompt the user until he provides a positive integer no greater than 8
height = 0
while height < 1 or height > 8:
    height = cs50.get_int("Height :")

# Generate the desired half-pyramid.
for i in range(height):
    print(" " * (height - 1 - i) + "#" * (i + 1) + "  " + "#" * (i + 1))
