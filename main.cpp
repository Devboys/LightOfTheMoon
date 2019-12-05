/*  
 * Created by Alberto Giudice on 07/11/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * + 
 * + Alberto Giudice [15/12/19] - Game start with singleton implemented
 * + Alberto Giudice [07/11/19] - vld implementation
 */

#include <vld.h>
#include "LightOfTheMoon.hpp"

int main(int argc, char** argv) {
	LightOfTheMoon::getInstance();
    return 0;
}