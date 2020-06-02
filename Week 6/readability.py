import re

text = input('Text: ')

letters = sum(character.isalpha() for character in text)
words = sum(character.isspace() for character in text) + 1
sentences = 0

for character in text:
    if re.match('^[?.!]$', character):  # must end with either ? / . / !
        sentences += 1

lettersOWords = letters / words * 100
sentencesOWords = sentences / words * 100
grade = round(0.0588 * lettersOWords - 0.296 * sentencesOWords - 15.8)

if grade < 1:
    print('Before Grade 1')
elif grade > 16:
    print('Grade 16+')
else:
    print(f'Grade {grade}')
