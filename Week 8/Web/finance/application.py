import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    """TODO"""
    # Get current cash
    row = db.execute("SELECT cash FROM users WHERE id = :user", user=session["user_id"])
    cash = usd(row[0]["cash"])

    # Get transactions
    rows = db.execute("SELECT username, company, UPPER(symbol) as symbol, SUM(quantity) as quantity FROM transactions WHERE username = (SELECT username FROM users WHERE id = :user) GROUP BY username, symbol", user=session["user_id"])
    stockPrice = {}
    totalPrice = {}

    if len(row) > 0 and len(rows) > 0:
        assets = row[0]["cash"]

        for row in rows:
            individualPrice = lookup(row["symbol"])["price"] * row["quantity"]

            assets += individualPrice
            totalPrice[row["symbol"].upper()] = usd(individualPrice)
            stockPrice[row["symbol"].upper()] = usd(lookup(row["symbol"])["price"])

        return render_template("index.html", cash=cash, transactions=rows, currentPrice=stockPrice, sumPrice=totalPrice, assets=usd(assets))
    elif len(row) > 0:
        assets = row[0]["cash"]

        return render_template("index.html", cash=cash, assets=usd(assets))

    return apology("Something went wrong!")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    """TODO"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        amount = request.form.get("shares")

        if symbol and int(amount) > 0:
            res = lookup(symbol)
            userInfo = db.execute("SELECT * FROM users WHERE id = :user", user=session["user_id"])
            username = userInfo[0]["username"]
            cash = userInfo[0]["cash"]

            if res:
                companyName = res["name"]
                price = res["price"]
                purchaseAmt = price * int(amount)

                if cash < purchaseAmt:
                    return apology("Insufficient funds!")
                else:
                    # Record transaction
                    transaction = "INSERT INTO transactions (username, symbol, quantity, price, company) VALUES (:username, :symbol, :qty, :price, :cName)"
                    db.execute(transaction, username=username, symbol=symbol.upper(), qty=amount, price=purchaseAmt, cName=companyName)

                    # Update user's cash
                    updateCash = "UPDATE users SET cash = cash - :price WHERE id = :user"
                    db.execute(updateCash, price=purchaseAmt, user=session["user_id"])

                    return redirect("/")
            else:
                return apology("Invalid symbol!")
        elif not symbol:
            return apology("Invalid symbol!")
        elif int(amount) < 1:
            return apology("Requires at least 1 stock quantity!")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    """TODO"""
    rows = db.execute("SELECT * FROM transactions WHERE username = (SELECT username FROM users WHERE id = :user)", user=session["user_id"])

    if len(rows) >= 0:
        print(rows)
        return render_template("history.html", history=rows, cvtUsd=usd)
    else:
        return apology("Something went wrong!")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    """TODO"""
    isQuoting = False

    if request.method == "GET" and not isQuoting:
        return render_template("quote.html", isQuoting=isQuoting)
    else:
        isQuoting = True
        symbol = request.form.get("symbol")

        if symbol:
            res = lookup(symbol)

            if res:
                companyName = res["name"]
                symbol = res["symbol"]
                price = usd(res["price"])
                return render_template("quote.html", isQuoting=isQuoting, companyName=companyName, symbol=symbol, price=price)
            else:
                return apology("Invalid symbol!")
        else:
            return apology("Invalid symbol!")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    """TODO"""
    # rows = db.execute("STATEMENT")
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        matchPassword = request.form.get("confirmation")

        if username and password and matchPassword:
            checkUser = db.execute("SELECT * FROM users WHERE username = :username", username=username)

            if len(checkUser) == 0:
                if password == matchPassword:
                    db.execute("INSERT INTO users (username, hash) VALUES (:username, :matchPW)", username=username, matchPW=generate_password_hash(matchPassword))
                    return redirect("/")

                elif password != matchPassword:
                    return apology("Password does not match!")

            elif len(checkUser) > 0:
                return apology("Username exists!")

        elif not username:
            return apology("Username input cannot be blank!")

        elif not password or not matchPassword:
            return apology("Passwords' input cannot be blank!")
    else:
        return render_template("register.html")

    return apology("Unable to register. Please try again later!")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    """TODO"""
    userId = session["user_id"]
    rows = db.execute("SELECT UPPER(symbol) as symbol, SUM(quantity) as quantity, company, username FROM transactions WHERE username = (SELECT username FROM users WHERE id = :user) GROUP BY symbol", user=userId)

    if request.method == "GET":
        return render_template("sell.html", foundStocks=rows)
    else:
        sellStock = request.form.get("symbol")
        sellCount = request.form.get("shares")

        if sellStock and int(sellCount) > 0:
            for row in rows:
                username = row["username"]
                company = row["company"]
                symbol = row["symbol"]
                qty = row["quantity"]

                if symbol == sellStock:
                    if qty >= int(sellCount):
                        # TODO, update transactions
                        price = lookup(symbol)["price"] * int(sellCount)

                        updateTransn = "INSERT INTO transactions (username, symbol, quantity, price, company) VALUES (:username, :symbol, :qty, :price, :cName)"
                        db.execute(updateTransn, username=username, symbol=symbol, qty=int(sellCount) * -1, price=price, cName=company)

                        updateCash = "UPDATE users SET cash = cash + :price WHERE id = :user"
                        db.execute(updateCash, price=price, user=userId)

                        return redirect("/")
                    else:
                        return apology("Too many shares!")
        elif not sellStock:
            return apology("Invalid symbol!")
        elif not sellCount or int(sellCount) <= 0:
            return apology("Invalid shares!")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
