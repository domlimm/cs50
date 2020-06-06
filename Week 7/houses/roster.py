# TODO
from sys import argv, exit
import cs50
import csv

if len(argv) != 2:
    print('Usage: python roster.py house')
    exit(1)

db = cs50.SQL('sqlite:///students.db')

retrieved = db.execute('SELECT * FROM students WHERE house = ? ORDER BY last ASC, first ASC', argv[1])

for row in retrieved:
    first = row['first']
    middle = row['middle']
    last = row['last']
    birth = row['birth']

    if middle != None:
        print(f'{first} {middle} {last}, born {birth}')
    else:
        print(f'{first} {last}, born {birth}')
