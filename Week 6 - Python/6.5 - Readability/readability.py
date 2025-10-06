import cs50


def main():
    # Ask for text
    text = cs50.get_string("Text: ")

    # Compute grade
    grade = get_grade(text)

    # Print grade
    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def get_grade(s):
    # Compute words_count
    words_count = s.count(" ") + 1

    # 1. Compute L (average number of letters per 100 words)
    letters_count = sum(char.isalpha() for char in s)
    L = letters_count / words_count * 100

    # 2. Compute S (average number of sentences per 100 words)
    sentences_count = s.count(".")
    sentences_count += s.count("?")
    sentences_count += s.count("!")
    S = sentences_count / words_count * 100

    # Compute Compute Coleman-Liau index
    grade = round(0.0588 * L - 0.296 * S - 15.8)
    return grade


main()
