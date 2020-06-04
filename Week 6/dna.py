from sys import argv, exit
import csv

if len(argv) != 3:
    print('Usage: python dna.py data.csv sequence.txt')
    exit(1)

csvFile = open(argv[1], 'r')

dnaFile = open(argv[2], 'r')
dna = dnaFile.read()

with csvFile:
    temp_csv = csv.reader(csvFile)

    patterns = next(temp_csv)
    patterns.pop(0)

    # Initialise dictionary to store no. of occurences
    # based off unique traits found in 'patterns'
    appearances = {}
    appearances = {pattern: 0 for pattern in patterns}

    # Find number of occurences for each dna trait
    for pattern in patterns:
        lenPattern = len(pattern)
        lenDna = len(dna)
        occurence = [0, 0]
        nextOccurence = False
        i = dna.find(pattern)

        if i < 0:
            continue

        while True:
            nextIndex = dna.find(pattern, i)

            if nextIndex != -1:
                if i == nextIndex:
                    if not nextOccurence:
                        occurence[0] += 1
                    else:
                        occurence[1] += 1

                    i = nextIndex + lenPattern
                elif nextIndex > i:
                    nextOccurence = True
                    occurence[1] = 1
                    i = nextIndex + lenPattern
                else:
                    break

            if occurence[0] != 0 and occurence[1] != 0:
                if occurence[1] > occurence[0]:
                    nextOccurence = False
                    occurence[0] = occurence[1]
                    occurence[1] = 0

            if nextIndex == -1:
                break

        appearances[pattern] = occurence[0]

    isMatch = False

    # print(appearances)

    # Iterate each row in csv file
    for row in temp_csv:
        isMatch = False

        # Compare each trait occurence of each individual against
        # total occurences found in dna
        for i in range(len(patterns)):
            if int(row[i + 1]) == appearances[patterns[i]]:
                isMatch = True
            else:
                isMatch = False
                break

        if isMatch == True:
            print(row[0])
            exit(0)

    if isMatch == False:
        print('No match')
