# NetworkingProject
# Network Programming Lab - Final Project Guidelines
# The final project is a hybrid P2P and client-server gaming platform with a game that uses it.
# First we describe the platform: The functional requirements of the platform are:
# Phaze 1: login 
# 1.New user can register to the system with a username and password
# 2.Existing user can login to the system with his username and password
# 3.The system will remember all the user and passwords even if the server is shutdown and restarted.
# Phaze 2: Matching
# 4.User can see the other users which are on line and available top lay with him.
# 5.A user can send a request to start a game to another user
# 6.The platform will enablea user to automatically start a game with a random available user
# (the user does not need to select the opponent user, the system will do it for him).
# 7. A user may accept or reject a game request if he was not the one who sent the request 
# or asked the platform to automatically do it for him.
# Phaze 3: Game play
# 8. After a matching of two players is done the system will enable the game app to send messages from
# one user to the other. 
# 9.Exchanging the messages between the game apps will be performed P2P using UDP protocol.
# 10.The game app can end the game and declare its result: user 1 won, user 2 won, draw, game cancelled.
# And the score for each user: for example winner: user 1, score 1: 20, score 2: 1011.
# After the game a user is able to see the score board and his position in it.
# The score board will ordered the users according to their accumulating score.
# Non functional requirements: 
# 1.The system should be a hybrid P2P system with central directory.
# 2.The communication with the central directory will be performed using TCP.
# 3.The game messages during a game will be transmitted P2P using UDP.
# 4.The password must be stored at the server on a file.
# 5.The password need to be saved as a hash instead of plain text.
# 6.The system must be written in C++
# 7.You are not allowed to use any external libraries like Boost or ACE...
# 8.You may and should use all the classes we have written during the course.
# 9.You should provide Unit test to your platform APIGame specification:
# The game is quites imple and it is for you to choose which game to implement.
# The game should be very simple with two players,no graphics is required.
# It can be as simple asX-Mix-Drix or hang-man or trivia or any other simple game that uses the platform.
# The project can be programmed by a group of Maximum two students!!!
# No exceptions will be permitted,any student that can't find a partner will do the project on his own.
