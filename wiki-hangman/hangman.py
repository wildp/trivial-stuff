# Copyright 2018 Peter Wild
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# hangman.py
# by Peter Wild
link = "https://github.com/wildp/trivial-stuff/tree/master/wiki-hangman"



from html.parser import HTMLParser
from urllib.request import urlopen
from string import ascii_lowercase, digits, punctuation


""" GLOBAL VARIABLES """
# note that pData is used as a global variable but is
# created just before use and then deleted
     
url = "https://en.wiktionary.org/wiki/Special:RandomInCategory/English_lemmas#English"
titleSuffix = " - Wiktionary"

#url = "https://en.wiktionary.org/wiki/Special:Random"
#url = "https://en.wikipedia.org/wiki/Special:Random"
#titleSuffix = " - Wikipedia"

# chars in ascii_lowercase are the acceptable letters - anything else is not permitted for use

asciiScaffold = [    # ascii art :)

                     ## All Lives
                     ["              ",
                      "              ",
                      "              ",
                      "              ",
                      "              "],
                      ## 9 Lives
                     ["              ",
                      "              ",
                      "              ",
                      "              ",
                      " ──────────── "],
                      ## 8 Lives
                     ["              ",
                      "     │        ",
                      "     │        ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 7 Lives
                     ["     ┌────    ",
                      "     │        ",
                      "     │        ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 6 Lives
                     ["     ┌────┐   ",
                      "     │        ",
                      "     │        ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 5 Lives
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │        ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 4 Lives
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │    █   ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 3 Lives
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │   /█   ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 2 Lives
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │   /█\  ",
                      "     │        ",
                      " ────┴─────── "],
                      ## 1 Life
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │   /█\  ",
                      "     │   /    ",
                      " ────┴─────── "],
                      ## No Lives
                     ["     ┌────┐   ",
                      "     │    O   ",
                      "     │   /█\  ",
                      "     │   / \  ",
                      " ────┴─────── "]]



""" CLASSES """

class Word:
    # Game related functions 

    def __init__(self, word, fancyWord=""):
        # word should only comprise of acceptable letters
        self.word   = word.lower()
        self.length = len(self.word)

        # list to store the status of each letter
        self.status = []
        
        # lists to store incorrect and correct entered letters
        self.failed = []
        self.right  = []
        
        # set up self.status
        for letter in self.word:
            if letter in ascii_lowercase:
                self.status.append("Status:NotFound")
            elif letter == ' ':
                self.status.append("Status:Blank")
            elif letter == "\'":
                self.status.append("Status:Apostrophe")
            else:
                self.status.append("Status:Punctuation")

        # set up the displayed word
        if fancyWord == "":
            self.displayWord = word
        else:
            self.displayWord = fancyWord
        return

    def check(self, c):
        # check if character exists within word and uncover if so
         success = False
         for i in range(self.length):
             if c == self.word[i] and self.status[i] == "Status:NotFound":
                 # mark all instances of char in word as found
                 self.status[i] = "Status:Found"
                 success = True

         if success:
             # add char to successful inputs
             self.right.append(c)
             return "Status:Found"     
         else:
             self.failed.append(c)
             # add char to failed inputs
             return "Status:NotFound"

    def check_if_win(self):
        # check if all letters have been uncovered
        victory = True
        for i in range(self.length):
            if self.status[i] == "Status:NotFound":
                victory = False
        return victory

    def clean(self, istring):
        # check validity of input character
        if len(istring) < 1:
            # prevent zero length strings from being converted
            return "Status:Error"

        # take only first char (ignore the rest)
        c = istring[0].lower()

        # check if char has been entered already
        if c in self.failed or c in self.right:
            return "Status:Error"

        if c in ascii_lowercase:
            # check if char is an acceptable letter
            return self.check(c)#

        return "Status:Error"

    def debug(self):
        # cheat message
        print("Word: "+self.word+"\nLen:  "+str(self.length)+"\n")
        return

    def display_incorrect(self):
        # displays incorrect letters
        if len(self.failed) != 0:
            ostring = "Incorrect letters:"
            self.failed.sort()
            for letter in self.failed:
                ostring += ' ' + letter 
            print(ostring)
        return

    def print(self):
        # displays word with blanks for covered letters
        ostring = " "

        for index in range(self.length):
            
            if self.status[index] == "Status:NotFound":
                ostring += "_"
            elif self.status[index] == "Status:Found":
                ostring += self.displayWord[index]
            elif self.status[index] == "Status:Blank":
                ostring += " "
           

            elif self.status[index] == "Status:Punctuation":
                ostring += self.displayWord[index]

            ostring += " "

            if self.status[index] == "Status:Apostrophe":
                ostring = "".join(ostring.rsplit("  "))
                if "s" in self.right:
                    ostring += "\'"
                else:
                    ostring += " "

        #end for loop

        print(ostring)
        return

    def reveal(self):
        # used for endgame if lost the game
        print("The word was:",self.displayWord)
        return


class ParserData:
    # a class to store data for use in Parser
    # currently is accessed as a global variable
    # from Parser() and is later deleted 

    def __init__(self):
        self.titletag = False
        self.title    = b""

    def get_title(self):
        #removes the end of the title from the word
        return "".join(self.title.rsplit(titleSuffix))


class Parser(HTMLParser):
    # Parses HTML to find a title
    # A global ParserData() object called pData must be createwd before use

    def handle_starttag(self, tag, attrs): 
        global pData
        ##print("<"+tag+">")
        if str(tag) == "title":
            #indicates if the current tag is a title
            pData.titletag = True
            
    def handle_endtag(self, tag):
        global pData
        ##print("<\\"+tag+">")
        if str(tag )== "title":
            pData.titletag = False
        
    def handle_data(self, data):
        global pData
        ##print(data)
        if pData.titletag == True:
            pData.title = data
    

""" FUNCTIONS """
            
def print_scaffold(lives, maxLives):
    # prints a scaffold for a certain number of lives
    index = maxLives - lives
    if index == 0:
        return
    print()
    for line in asciiScaffold[index]:
        print(line)
    return

def extract_word():
    # create instance of parser 
    parser = Parser()
    # create instance of ParserData
    global pData
    pData  = ParserData()

    # get html from website
    response = urlopen(url)
    html = response.read()
    
    # read html and return cleaned title
    parser.feed(html.decode("utf-8", errors="ignore"))
    word = pData.get_title()
    del pData
    return word

def clean(word):
    newWord = ""
    for letter in word.lower():
        if letter == " ":
            newWord += " "
        elif letter in ["a","à","á","â","ä","å","æ","ā","ă","ą","ά","α","ắ","ã"]:
            newWord += "a"
        elif letter in ["b","ƀ","ƃ","ƅ","β"]:
            newWord += "b"
        elif letter in ["c","ç","ć","ĉ","ċ","č","ƈ","χ"]:
            newWord += "c"
        elif letter in ["d","ď","đ","δ"]:
            newWord += "d"
        elif letter in ["e","è","é","ê","ë","ð","ē","ĕ","ė","ę","ě","έ","ή","ε","η","ə"]:
            newWord += "e"
        elif letter in ["f","ƒ"]:
            newWord += "f"
        elif letter in ["g","ĝ","ğ","ġ","ģ","γ"]:
            newWord += "g"
        elif letter in ["h","ĥ","ħ"]:
            newWord += "h"
        elif letter in ["i","ì","í","î","ï","ĩ","ī","ĭ","į","ı","ĳ","ί","ι","ϊ","i"]:
            newWord += "i"
        elif letter in ["j","ĵ"]:
            newWord += "j"
        elif letter in ["k","ķ","ĸ","κ"]:
            newWord += "k"
        elif letter in ["l","ĺ","ļ","ľ","ł","λ"]:
            newWord += "l"
        elif letter in ["m","μ"]:
            newWord += "m"
        elif letter in ["n","ñ","ń","ņ","ň","ŋ","ν"]:
            newWord += "n"
        elif letter in ["o","ò","ó","ô","õ","ö","ø","ō","ŏ","ő","œ","ɔ","ο","ω","ό","ώ"]:
            newWord += "o"
        elif letter in ["p","π","φ","ψ"]:
            newWord += "p"
        elif letter in ["q"]:
            newWord += "q"
        elif letter in ["r","ŕ","ŗ","ř","ρ"]:
            newWord += "r"
        elif letter in ["s","ś","ŝ","ș","š","ſ","ß","ς","σ","ş"]:
            newWord += "s"
        elif letter in ["t","þ","ţ","ț","ť","ŧ","θ","τ"]:
            newWord += "t"
        elif letter in ["u","ù","ú","û","ü","ũ","ū","ŭ","ů","ű","ų","ΰ","υ","ϋ","ύ","ủ"]:
            newWord += "u"
        elif letter in ["v"]:
            newWord += "v"
        elif letter in ["w","ŵ"]:
            newWord += "w"
        elif letter in ["x","ξ"]:
            newWord += "x"
        elif letter in ["y","ý","ÿ","ŷ"]:
            newWord += "y"
        elif letter in ["z","ź","ż","ž","ζ"]:
            newWord += "z"
        elif letter in punctuation or letter in digits or \
         letter in ["–","¡","¢","£","¤","¥","¦","§","©","»","¿","«",
                    "¬","­","®","¯","°","±","¿","·","¶","×"]:
            newWord += letter
        else:
            return ""
    return newWord

def game(extractedWord, displayedWord):
    # main game loop
    # word must be extracted
    w = Word(extractedWord, displayedWord)
    
    #w.debug()

    loop    = True
    gameWin = False
    lives   = 10

    while loop:

        # display word with underscores for unrevealed letters
        w.print()
        # display incorrect chars
        w.display_incorrect()

        # create variable to track staus of entered char 
        returnVal = "Status:Error"

        while returnVal == "Status:Error":
            # prompt for input and check if valid
            istring = input("Enter a letter: ")
            returnVal = w.clean(istring)

        if returnVal == "Status:NotFound":
            # decrement lives counter
            lives -= 1
            print_scaffold(lives, 10)

        if returnVal == "Status:NotFound" or lives <= 1:
            # display number of lives
            print("\nYou have",lives,"lives left.")

        if w.check_if_win():
            # check if win the game
            # break loop
            loop    = False
            gameWin = True

        elif lives <= 0:
            # check if lost the game
            # break loop
            loop    = False
            gameWin = False   

        print('\n')

    # end while loop

    if gameWin == True: 
        w.print()
        print("\n\nYou have won the game")

    if gameWin == False:
        print("You have lost the game")
        w.reveal()

    print('\n')
    return

def replay():
    # prompt asking user if they want to replay
    while True:
        put = input("Do you want to play again? (y/n): ")
        if put == 'y':
            return True
        if put == 'n':
            return False

def main():
    loop = True
    print("Wiki Hangman\n"+link+"\n\nWord Source:",url,"\n")
    
    try:
        while loop:
            word = extract_word()
            newWord = clean(word)

            if newWord == "":
                print("REJECTED:",word)
                continue
        
            print("NEW GAME:\n\n")
            game(newWord, word)
            loop = replay()
            print('\n')
            
    except Exception as e:
        print("Error:",e)
               
    return

if __name__ == "__main__":
    main()
