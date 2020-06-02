"""
From Problem Set 1:
American Express uses 15-digit numbers,
MasterCard uses 16-digit numbers,
and Visa uses 13- and 16-digit numbers

All American Express numbers start with 34 or 37;
most MasterCard numbers start with 51, 52, 53, 54, or 55
and all Visa numbers start with 4.

"""

amex = ['34', '37']
mcard = ['51', '52', '53', '54', '55']

credit_num = input('Number: ')

if len(credit_num) == 15 and credit_num[:2] in amex:
    print('AMEX')
elif len(credit_num) == 16:
    if credit_num[:2] in mcard:
        print('MASTERCARD')
    elif credit_num[0] == '4':
        print('VISA')
elif len(credit_num) == 13:
    if credit_num[0] == '4':
        print('VISA')
else:
    print('INVALID')
