# SSHSurvey

I wanted to create a product that would allow for the SSH protocol to be automated via files that were formatted in a special way. While doing this I also wanted to make it look as if a real user was conducting the actions. Some devices that have SSH running do not allow for new channels (Multiplexing) to happen. Using the libssh API at libssh.org, I am able to programmatically make commands always look like they are from a user. An advantage of using this program will also remove the need for elevating privileges everytime you use a new channel, because I am essentially using the same channel, the same connection that you would be if you selected interactive shell in the menu option on here. 

I have also opted for a menu like interface here where selections are made by the number that is adjacent to the menu option, this makes it easy for users to select what they want to do, if you want to have a totally hands off approach, I am building in a way where the file just starts from the beginning in a certain way, I am still actively working on this, so the menu (and soon submenus) will be built the more I have time for it.


For any questions feel free to message me at mclarkfyrue@gmail.com.

