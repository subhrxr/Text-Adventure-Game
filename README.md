# Text-Adventure-Game


   ***ABOUT THE GAME***

1. you are trapped in a maze consisting of different rooms.
2. you can kill an enemy using 'kill enemy_name' command.
3. You can move to another room using the 'go direction' command. Where the directions can be north , east , west , south.
4. Always use small letters for the commands.
5. When you move into a room with an enemy and try to kill it then,
      a) The enemy dies if you have the required weapons to kill the enemy.
      b) You do not have the required weapons and the enemy attacks you so you loose health points. and the enemy flees to 
         another room(it may as well stay in the same room if it wants).
      c) You do not attack the enemy and try to run away. In which case there is a 50-50 chance that the enemy will attack
         you and it will not go to another room.
6. You can pick items using the 'pick object_name' command.
7. When you use an object to kill an enemy, it can't be used again.
8. You can check the current objects that you possess using the 'list' command.
9. You can use the 'help' command to see which commands you can use.
10.You can use the 'status' command to see your current status. Info like enemies killed, health, rooms visited, etc will be
   displayed.
11.The game ends when,
       a) You kill all the enemies.
       b) You loose all your health.
       c) You go inside a room with no doors.
12. Good luck!!!

** HOW TO INSTALL**

I have written a CMakeLists.txt file. In Linux, it can be compiled as follows...

1. Unzip the zipped folder finaltask.zip
2. Go to that directory using your terminal.
3. Run 'cmake -S . -B .'  (excluding the quotes)
4. If you get an error message that the system does not recognise 'cmake'. In this case, you'll have to run 
   'sudo pacman -S cmake' (if you are using arch or manzaro), if you are using any other linux version then use it's
   own install command.
5. When you are done with step 3(and 4), run 'make'(excluding the quotes)
6. There are four maps provided, you can use any of them.
7. To run, use the following command.
8. ./main map.json
9. In place of map.json you can use any map that is provided or maybe build your own map as well.


***HAVE FUN***
