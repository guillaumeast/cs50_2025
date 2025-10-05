import cs50

# Coin values
coins = [25, 10, 5, 1]


def main():
    # Prompt user for change
    change = 0
    while change <= 0:
        change = cs50.get_float("Change: ") * 100

    # Compute how many coins are needed for change
    coin_count = 0
    while change > 0:
        coin_value = get_coin(change)
        coin_count += 1
        change -= coin_value

    # Print result
    print(coin_count)


def get_coin(max_value):
    for coin in coins:
        if coin <= max_value:
            return coin


main()
