# CL-Chess-in-C
My first attempt to make Command Line Chess, as simply as I can in C. Does not handle castling or en passant
![alt text](https://github.com/harshbanthiya/CL-Chess-in-C/blob/main/imgs/cl-chess2.png?raw=true)
![alt text](https://github.com/harshbanthiya/CL-Chess-in-C/blob/main/imgs/cl-chess1.png?raw=true)

## USAGE
```sh
git clone https://github.com/harshbanthiya/CL-Chess-in-C.git ; cd CL-Chess-in-C ; gcc CL_Chess.c -o chess 
```
* Run the program 
```sh
./chess 
```
## How to play

### This will print the board 
```sh
> D 
```

### Type the move you wanna make in the this format (source square position) (destination square position)
```sh
> e2e4 
```


### If the move you wanna make will lead to a promotion you can add a letter at the end to specify what you would like to promote to (q, r, b, n)
```sh
> a7a8q
```

### To Quit
```sh
> exit
```
