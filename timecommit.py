# Title: timecommit.py
# Purpose: To automate a git push to origin in the current directory. With a commit message containing the user's name and the current time and date.
# Developer: Cameron Palmer

import time
import os
from datetime import datetime
from datetime import date as dt

choice = int(input("Would you like name, time and date for your commit (1), or an alternate message? (2)\n>"))

if (choice == 1):
	name = input("Enter your name\n > ")
	today = dt.today()
	d2 = today.strftime("%B %d, %Y")

	now = datetime.now()
	time_string = now.strftime("%H:%M:%S")

	os.system("git add .")
	os.system("git commit -m " + '"Commit by ' + name + ' on ' + d2 + ' ' + time_string +  '"')
	os.system("git push origin master")

elif (choice == 2):
	message = input("Enter a commit message\n>")
	os.system("git add .")
	os.system("git commit -m " + '"' +  message + '"')
	os.system("git push origin master")	
else:
	print("You failed!")
