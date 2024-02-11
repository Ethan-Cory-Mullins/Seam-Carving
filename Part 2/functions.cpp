#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"

using std::cout, std::endl, std::string, std::ifstream, std::ofstream;

void initializeImage(Pixel image[][MAX_HEIGHT]) {
  // iterate through columns
  for (unsigned int col = 0; col < MAX_WIDTH; col++) {
    // iterate through rows
    for (unsigned int row = 0; row < MAX_HEIGHT; row++) {
      // initialize pixel
      image[col][row] = {0, 0, 0};
    }
  }
}

void loadImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int& height) {

  initializeImage(image);
  unsigned int max_color, i, j, end; // ints to iterate through and to check if its the end of the file 
  short red, green, blue; // colors that take the value of the thre pixel variables 
  string filetype = "";  
  ifstream inFS; // open the file 
  inFS.open(filename);
  if (!inFS.is_open()) { // checks if the file is open 
    throw std::runtime_error("Failed to open " + filename); // makes sure the file opened properlly 
  }
  inFS >> filetype; // takes in the first input (p3 or P3)
  if (filetype != "p3" && filetype != "P3") { // if its not p3 or P3 then throw an errlr 
    throw std::runtime_error("Invalid type " + filetype);
  }
  inFS >> width;
  inFS >> height; // all of these take in the inputs in the file that describe the aspects of the file 
  inFS >> max_color;
  if ( width > MAX_WIDTH  || height > MAX_HEIGHT) { // makes sure that height and width arnt too low or high 
    throw std::runtime_error("Invalid dimensions");
  }
  for (i = 0; i < height; i++) { // loop through the columns
    for (j = 0; j < width; j++) { // loop throug the rows
      if (inFS.fail() || inFS.eof()) {
        throw std::runtime_error("Invalid color value");
      }
      inFS >> red;
      if (inFS.fail() || inFS.eof()) {
        throw std::runtime_error("Invalid color value"); // all of these calls make sure that all the colors numners are valid 
      }
      inFS >> green;
      if (inFS.fail() || inFS.eof()) {
        throw std::runtime_error("Invalid color value");
      }
      inFS >> blue;
      if (red < 0 || green < 0 || blue < 0 || red > 255 || green > 255|| blue > 255) {
        throw std::runtime_error("Invalid color value");
      }
      if (inFS.fail()) {
        throw std::runtime_error("Invalid color value");
      }
      image[j][i] = {red, green, blue}; // finally, when all the inputs have been validated, then the (j,i) pixel can be updated 
    }

  }
  inFS >> end; // makes an error if there isn;t anything else, so i can check if it is the end of the fiel 
  if (inFS.eof() && inFS.fail()) {
  } else {
    throw std::runtime_error("Too many values");
  }
  


  inFS.close(); // closes the file 

}

void outputImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height) {
  ofstream outFS; 
  outFS.open(filename);// opens a new file with ofsteam to output into 
  if (!outFS.is_open()) {
    throw std::runtime_error("Faild to open " + filename); // makes sure file is open 
  }
  unsigned int i,j; // ints to interate thorugh 
  outFS << "P3" << endl;
  outFS << width << " " << height << endl; // formats the beginning 
  outFS << "255" << endl;
  for (i = 0; i < height; i ++) { // loop through the coloumns 
    for (j = 0; j < width; j ++) { /// loop through the rows 
      outFS << (image[j][i]).r << ' ';
      outFS << (image[j][i]).g << ' ';
      outFS << (image[j][i]).b << ' '; // outputs the r,g,and b components with a space in between them 
    }
    outFS << endl; // starts a new line when it switches to the next collumn
  }
  outFS.close();
}

unsigned int energy(Pixel image[][MAX_HEIGHT], unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
  unsigned int energy;
  int x_r_diff, x_g_diff, x_b_diff, y_r_diff, y_g_diff, y_b_diff, x_grad, y_grad;
  if (x == 0) { // all of these are the same function, but with differnt indexes
    x_r_diff = image[x+1][y].r - image[width-1][y].r;
    x_g_diff = image[x+1][y].g - image[width-1][y].g;
    x_b_diff = image[x+1][y].b - image[width-1][y].b;
    x_grad = (x_r_diff) * (x_r_diff) + (x_g_diff) * (x_g_diff) + (x_b_diff) * (x_b_diff);

  } else if (x == width-1) {
    x_r_diff = image[0][y].r - image[x-1][y].r;
    x_g_diff = image[0][y].g - image[x-1][y].g;
    x_b_diff = image[0][y].b - image[x-1][y].b;
    x_grad = (x_r_diff) * (x_r_diff) + (x_g_diff) * (x_g_diff) + (x_b_diff) * (x_b_diff);

  } else { // this is for the normal casees 
    x_r_diff = image[x+1][y].r - image[x-1][y].r;
    x_g_diff = image[x+1][y].g - image[x-1][y].g;
    x_b_diff = image[x+1][y].b - image[x-1][y].b;
    x_grad = (x_r_diff) * (x_r_diff) + (x_g_diff) * (x_g_diff) + (x_b_diff) * (x_b_diff); 
  }
  if (y == 0){ 
    y_r_diff = image[x][y+1].r - image[x][height-1].r;
    y_g_diff = image[x][y+1].g - image[x][height-1].g;
    y_b_diff = image[x][y+1].b - image[x][height-1].b;
    y_grad = (y_r_diff) * (y_r_diff) + (y_g_diff) * (y_g_diff) + (y_b_diff) * (y_b_diff);
  } else if (y == height-1) {
    y_r_diff = image[x][0].r - image[x][y-1].r;
    y_g_diff = image[x][0].g - image[x][y-1].g;
    y_b_diff = image[x][0].b - image[x][y-1].b;
    y_grad = (y_r_diff) * (y_r_diff) + (y_g_diff) * (y_g_diff) + (y_b_diff) * (y_b_diff);
  } else { // this is for the normal case 
    y_r_diff = image[x][y+1].r - image[x][y-1].r;
    y_g_diff = image[x][y+1].g - image[x][y-1].g;
    y_b_diff = image[x][y+1].b - image[x][y-1].b;
    y_grad = (y_r_diff) * (y_r_diff) + (y_g_diff) * (y_g_diff) + (y_b_diff) * (y_b_diff);
  }

  energy = y_grad + x_grad; // finaly, no matter if it was an edge case or not for both x or y or x and y, it computes
                            // the energy of the specific pixel, i would have made a loop for it, but it was only for one specific pixel
                            // so for me it was easier to just copy paste the first one and change how it indexes :) !!!
  
  return energy;
}

unsigned int loadVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_col, unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int i, col, mid_energy, left_energy, right_energy, tot_energy;
  tot_energy = 0;
  seam[0] = start_col;
  tot_energy += energy(image, start_col, 0, width, height);
  col = start_col;
  for (i = 1; i < height; i ++) {
    if (col == 0) { // cant go right 
      mid_energy = energy(image, col, i, width, height);
      left_energy = energy(image, col + 1, i, width, height);
      if (left_energy < mid_energy) { // go left 
        col += 1;
      } // any other case, go middle so you dont have to update col 
    } else if (col == (width -1)) { // can't go left 
      mid_energy = energy(image, col, i, width, height);
      right_energy = energy(image, col - 1, i, width, height);
      if (right_energy < mid_energy) { // go right 
        col -= 1;
      } // any other case, go middle so you dont have to update col 
    } else { // can go any way 
      mid_energy = energy(image, col, i, width, height);
      left_energy = energy(image, col + 1, i, width, height);
      right_energy = energy(image, col - 1, i, width, height);
      if ((left_energy < mid_energy) && (left_energy < right_energy)) { // go left 
        col += 1;
      } else if ((right_energy < mid_energy) && (right_energy < left_energy)) { // go right 
        col -= 1;
      } else if ((left_energy < mid_energy) && (left_energy == right_energy)) { // go left 
        col += 1;
      }
    }
    seam[i] = col;
    tot_energy += energy(image, col, i, width, height);
  }
  return tot_energy;
}

unsigned int loadHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_row, unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int i, row, middle_energy, down_energy, up_energy, tot_energy;
  tot_energy = 0;
  seam[0] = start_row;
  row = start_row;
  tot_energy += energy(image, 0, start_row, width, height);
  for (i = 1; i < width; i ++) {
    if (row == 0) { // cant go up 
      middle_energy = energy(image, i, row, width, height);
      down_energy = energy(image, i, row + 1, width, height);
      if (down_energy < middle_energy) { // go down 
        row += 1;
      } // any other case, go middle so you dont have to update row 
    } else if (row == (height -1)) { // can't go down 
      middle_energy = energy(image, i, row, width, height);
      up_energy = energy(image, i, row - 1, width, height);
      if (up_energy < middle_energy) { // go up 
        row -= 1;
      } // any other case, go middle so you dont have to update row 
    } else { // can go any way 
      middle_energy = energy(image, i, row, width, height);
      down_energy = energy(image, i, row + 1, width, height);
      up_energy = energy(image, i, row - 1, width, height);
      if ((down_energy < middle_energy) && (down_energy < up_energy)) { // go down 
        row += 1;
      } else if ((up_energy < middle_energy) && (up_energy < down_energy)) { // go up 
        row -= 1;
      } else if ((up_energy < middle_energy) && (up_energy == down_energy)) { // go up 
        row -= 1;
      }
    }
    seam[i] = row;
    tot_energy += energy(image, i, row, width, height);
  }
  return tot_energy;
}

void findMinVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int i, lowest_energy, col_energy, col;
  lowest_energy = loadVerticalSeam(image, 0, width, height, seam);
  col = 0;
  for (i = 1; i < width; i++) {
    col_energy = loadVerticalSeam(image, i, width, height, seam);
    if (col_energy < lowest_energy) {
      lowest_energy = col_energy;
      col = i;
    }
  }
  loadVerticalSeam(image, col, width, height, seam);
}

void findMinHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int i, lowest_energy, row_energy, row;
  lowest_energy = loadVerticalSeam(image, 0, width, height, seam);
  row = 0;
  for (i = 1; i < height; i++) {
    row_energy = loadHorizontalSeam(image, i, width, height, seam);
    if (row_energy < lowest_energy) {
      lowest_energy = row_energy;
      row = i;
    }
  }
  loadHorizontalSeam(image, row, width, height, seam);
}


void removeVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int height, unsigned int verticalSeam[]) {
  unsigned int i, j;
  for (i = 0; i < height; i++) {
    for (j = (verticalSeam[i]); j < (width - 1); j++) {
      image[j][i] = image[j+1][i];
    }
  }
  width -= 1;
}

void removeHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int& height, unsigned int horizontalSeam[]) {
  unsigned int i, j;
  for (i = 0; i < width; i++) {
    for (j = (horizontalSeam[i]); j < (height - 1); j++) {
      image[i][j] = image[i][j+1];
    }
  }
  height -= 1;
}
