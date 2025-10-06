from cs50 import get_string


def main():
    number = get_string("Number: ")

    # Split number into digits (reversed order!)
    digits = [int(c) for c in number[::-1]]
    digits_count = len(digits)

    # Check Luhn's Algorithm
    checksum = 0

    # 1. Multiply half of the digits by 2 and sum those products' digits together
    # Starting with the number’s second-to-last digit
    for i in range(1, digits_count, 2):
        # Multiply current digit by 2
        doubled = digits[i] * 2
        # If results is 2 digits long => sum those digits
        if doubled > 9:
            doubled = 1 + (doubled % 10)
        # Add the final result to checksum
        checksum += doubled

    # 2. Add the digits that weren't multiplied by 2
    for j in range(0, digits_count, 2):
        checksum += digits[j]

    # 3. Check last digit (should be 0)
    if checksum % 10 == 0:
        first = digits[digits_count - 1]
        second = digits[digits_count - 2]
        first_two_digits = first * 10 + second
        if first == 4 and digits_count in (13, 16):
            print("VISA")
        elif first_two_digits in (34, 37) and digits_count == 15:
            print("AMEX")
        elif 50 < first_two_digits < 56 and digits_count == 16:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


main()
