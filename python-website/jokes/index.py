#!/usr/bin/env python
import random

def generate_html_page(joke):
	html = f'''
	<!DOCTYPE html>
	<head>
		<html lang="en">
		<meta charset="UTF-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<link rel="stylesheet" type="text/css" href="../styles.css">
		<title>Random Joke Generator</title>
	</head>
	<body>
		<h1>Random Joke Generator</h1>
		<p>{joke}</p>
		<button onclick="window.location.reload();">Tell me another joke!</button>
		<p> </p>
		<p class="subscript">This is used to test a CGI script as the default file for a directory. Also some wierdness with bytestream encoding.</p>
	</body>
	</html>
	'''
	return html

jokes = [
	"Why don't scientists trust atoms? Because they make up everything!",
	"Did you hear about the mathematician who's afraid of negative numbers? He will stop at nothing to avoid them!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the scarecrow win an award? Because he was outstanding in his field!",
	"Why did the bicycle fall over? It was two-tired!",
	"Why don't eggs tell jokes? Because they might crack up!",
	"What do you call a bear with no teeth? A gummy bear!",
	"Why did the tomato turn red? Because it saw the salad dressing!",
	"What do you call a fish wearing a crown? King Neptune!",
	"Why did the golfer bring two pairs of pants? In case he got a hole in one!",
	"Why do programmers prefer dark mode? Because light attracts bugs.",
	"Why did the web developer walk out of a restaurant in disgust? The seating was laid out in tables.",
	"Why did the web developer always carry a pencil and paper? In case they needed to draw a website.",
	"Why was the website cold? It left its CSS in the draft.",
	"Why was the HTML element so self-conscious? It had low self-closing tags.",
	"Why do programmers prefer iOS development over Android development? Because on iOS, there are no Java exceptions.",
	"Why did the developer always carry a spoon? In case they needed to stir some Java.",
	"Why did the programmer get stuck in the shower? They forgot to escape the soap.",
	"Why did the computer go to the doctor? It had a virus and needed JavaScript.",
	"Why did the developer break up with their significant other? They didn't get enough commits.",
	"Why did the programmer refuse to play cards with the deck? They prefer arrays.",
	"Why did the web developer go broke? They couldn't find their cache.",
	"Why did the programmer go broke? They spent all their money on debugging tools.",
	"Why did the web developer refuse to ride the roller coaster? They didn't want to be stuck in an infinite loop.",
	"Why did the programmer wear glasses while coding? They wanted to see."
]

# Select a random joke
random_joke = random.choice(jokes)

# Generate HTML page with the random joke
generated_html = generate_html_page(random_joke)

# Print CGI header and generated HTML
print("Content-Type: text/html; charset=UTF-8")
print()
print(generated_html)
