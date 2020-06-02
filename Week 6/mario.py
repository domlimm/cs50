def main():
    isNumber = False

    while not isNumber:
        try:
            size = int(input('Height: '))

            if size > 0 and size <= 8:
                isNumber = True
                break
        except ValueError:
            isNumber = False

    build(size, size)

def build(size, counter):
    spaces = size - 1

    if size == 0:
        return 1
    else:
        print(' ' * spaces, end='')
        print('#' * (counter - spaces), end='  ')
        print('#' * (counter - spaces), end='\n')

        return build(size - 1, counter)

main()
