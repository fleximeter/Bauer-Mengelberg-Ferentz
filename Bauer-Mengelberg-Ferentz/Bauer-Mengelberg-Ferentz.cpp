/*
* File: Bauer-Mengelberg-Ferentz.cpp
* Author: Jeff Martin
* This file contains the main() function for running the Bauer-Mengelberg-Ferentz
* algorithm for finding eleven-interval row generators.
*
* Copyright © 2022 by Jeffrey Martin. All rights reserved.
* Email: jmartin@jeffreymartincomposer.com
* Website: https://jeffreymartincomposer.com
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "bauer_algorithm.h"
#include "bauer_trichord.h"
#include "bauer_trichord_ten.h"
#include "bauer_trichord_babbitt.h"

int main()
{
	generateTenTrichordRows("D:\\ten_trichord.json");
	return 0;
}
