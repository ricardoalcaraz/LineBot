
MazeValues enterRoom(MazeValues data) {
  data = stepInMaze(data);
  data = roomInMaze(data, theMaze);
  return data;
}

const char room1Decider[6] = {'S','S', 'L', 'L', 'S', 'L'};
char whichWay(MazeValues data) {
  uint8_t roomType = 0;
  static uint8_t n = 0;
  roomType |= (leftWall(data) << 2);
  roomType |= (hitWall(data) << 1);
  roomType |= rightWall(data);
  switch(roomType) {
    case 0: return 'S';
    break;
    case 1: n++;
            return room1Decider[n];
            break;
    case 2: return 'L';
    break;
    case 3: return 'L';
    break;
    case 4: return 'R';
    break;
    case 5: return 'S';
    break;
    case 6: return 'R';
    break;
    case 7: return 'T';
    break;
  }
}

/*Turn in maze
 * Returns the MazeValues data structure with updated direction and turn
 *  none  right left  turn around
 *    0   0b01  0b10  0b11
 */
MazeValues turnInMaze(MazeValues test) {
  uint8_t index = (test.turn << 2) + test.dir;                //Multiply the turn value by 4 and add the dir
  test.turn = 0x00;                                           //Reset the turn        
  test.dir = turn_table[index];                               //Grab the corresponding turn from a lookup table
  return test;                                                //Return current direction
}

/* Step in Maze
 * Returns the MazeValues data structure with update coordinates
 */
MazeValues stepInMaze(MazeValues test) {
  const int8_t rowMap[] = {+1, 0,  0, -1};
  const int8_t colMap[] = {0, +1, -1,  0};
  test.coord.row = (uint8_t) ((int8_t) test.coord.row + rowMap[test.dir]);
  test.coord.col = (uint8_t) ((int8_t) test.coord.col + colMap[test.dir]);
  return test;
}

/* Room in Maze
 * Returns the MazeValues data structure with update room and bees
 */
MazeValues roomInMaze(MazeValues test, const uint8_t maze[]) {
  uint16_t index = (test.coord.row * 21) + test.coord.col;      //Find the index that our current row and column gives us
  uint8_t maze_data = pgm_read_word_near(maze + index);           //Saving the maze data into a temp variable
  Serial.println(index);
  Serial.println(maze_data,HEX);
  test.bees += (maze_data & 0xF0);                                //Clearing the lower nibble to grab the bees
  test.room = (maze_data & 0x0F);                               //Clearing the upper nibble to grab the room
  return test;
}


/* Did you hit a wall?
*  Called from WhichWay subroutine
*  Inputs: Structure MazeValues
*  Output: Boolean
*/
//     Compass    S    E    W    N
//         dir   00   01   10   11
boolean hitWall(MazeValues mazeData) {
  const uint8_t hitTable[] = {0x08, 0x02, 0x04, 0x01};    //room bits
  uint8_t wall = hitTable[mazeData.dir];                  //grab the specific room that will cause a hit
  return (mazeData.room & wall);                          //Bitwise AND to check if there is a collision
}

/*Is there a right wall?
* Called from main loop
* Turns right then calls hit wall routine then restores original direction
* Inputs: Structure MazeValues
* Output: Boolean
*   none  right left  turn around
*   0     0b01  0b10  0b11
*/
boolean rightWall(MazeValues mazeData) {
  uint8_t tempDir = mazeData.dir;
  mazeData.turn = 0b01;                   //Set a right turn
  mazeData = turnInMaze(mazeData);
  boolean rightWall = hitWall(mazeData);  //Call hitwall function
  mazeData.dir = tempDir;                 //Restore original direction
  return rightWall;
}

/*Is there a left wall?
* Called from main loop
* Turns left and calls hit wall routine then restores original direction
* Inputs: Structure MazeValues
* Output: Boolean
*   none  right left  turn around
* 0   0b01  0b10  0b11
*/
boolean leftWall(MazeValues mazeData) {
  uint8_t tempDir = mazeData.dir;
  mazeData.turn = 0b10;                   //Set a left turn and turn
  mazeData = turnInMaze(mazeData);
  boolean leftWall = hitWall(mazeData);   //Call hitwall and check if the wall is there
  mazeData.dir = tempDir;                 //restore original direction and exit
  return leftWall;
}

/*Are we in the forest?
* Called from main loop will put the mcu to sleep if it is
* Inputs: Structure MazeValues
* Output: Boolean
*/
void inForest(MazeValues mazeData) {
  if(mazeData.coord.row == 0 && mazeData.coord.col == 0) {
    victorySpin(mazeData);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
  }
}

/*We are in the forest so spin!
* Called from main loop will spin the bot a few times
* Inputs: Structure MazeValues
* Output: Boolean
*/
void victorySpin(MazeValues mazeData) {
  for(uint8_t i = 0; i < mazeData.bees; i++){
    //Movement hasn't been integrated yet so pseudocode is used for now
    //turnAround
    //turnAround
  }
}

