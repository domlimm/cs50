# TODO
from sys import argv, exit
import cs50
import csv

if len(argv) != 2:
    print('Usage: python import.py data.csv')
    exit(1)

dataFile = open(argv[1], 'r')

db = cs50.SQL('sqlite:///students.db')

woMiddle = 'INSERT INTO students (first, middle, last, house, birth) VALUES (?, NULL, ?, ?, ?)'
wMiddle = 'INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)'

with dataFile:
    reader = csv.DictReader(dataFile)

    for row in reader:
        fullName = row['name'].split()

        if len(fullName) == 2:
            db.execute(woMiddle, fullName[0], fullName[1], row['house'], row['birth'])
        elif len(fullName) == 3:
            db.execute(wMiddle, fullName[0], fullName[1], fullName[2], row['house'], row['birth'])
