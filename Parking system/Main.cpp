#include <iostream>
#include <vector>
#include <conio.h>
#include "ParkingSystemClass.h"

using namespace std;

int convertIndToMin(int ind) {
	int time = 0;

	switch (ind) {
	case 1: time = 15; break;  
	case 2: time = 30; break;
	case 3: time = 45; break;
	case 4: time = 60; break;
	case 5: time = 75; break;
	case 6: time = 90; break;
	case 7: time = 120; break;
	case 8: time = 150; break;
	case 9: time = 180; break;
	case 10: time = 240; break;
	case 11: time = 360; break;
	case 12: time = 720; break;
	case 13: time = 1440; break;
	}

	return time;
}

float convertMinToCoins(int min, float price) {
	switch (min) {
	case 15: price /= 4; break;
	case 30: price /= 2; break;
	case 45: price = price - (price / 4); break;
	case 60: break;
	case 75: price *= 1.25f; break;
	case 90: price *= 1.50f; break;
	case 120: price *= 2; break;
	case 150: price *= 2.50f; break;
	case 180: price *= 3; break;
	case 240: price *= 4; break;
	case 360: price *= 6; break;
	case 720: price *= 12; break;
	case 1440: price *= 24; break;
	}

	return price;
}

void reserveSpotMenu(ParkingSystem& psv) {
	system("cls"); 
	psv.printParkingAreas();

	int idArea, idSpot, timeReserved;
	float totalPrice;

	cout << "ParkingId: ";
	cin >> idArea;
	try {
		psv[idArea];
	}
	catch (int e) {
		if (e == -1) {
			cout << "Invalid ParkingId!\n";
			getch();
			return;
		}
	}

	if (psv[idArea].getCapacity() == psv[idArea].getPsInUse()) {
		cout << "No more spots available!\n";
		getch();
		return;
	}

	cout << "SpotId: ";
	cin >> idSpot;
	if (idSpot < 0 || idSpot > psv[idArea].getCapacity()) {
		cout << "Invalid SpotId!\n";
		getch();
		return;
	}
	if (psv[idArea][idSpot - 1].inUse) {
		cout << "Spot is reserved!\n";
		getch();
		return;
	}

	if (psv[idArea].getType() == pType::Blue) {
		cout << "Time (15  30  45  1h  1:15h  1:30h  2h  2:30h  3h  4h  6h  12h  24h)\n";
		cout << "Val:   1   2   3   4    5      6     7    8     9  10  11   12   13\n";
	}

	if (psv[idArea].getType() == pType::Purple) {
		cout << "Time (15 30 45 1h 1:15h 1:30h 2h 2:30h 3h)\n";
		cout << "Val:  1   2  3  4   5     6    7   8    9\n";
	}

	if (psv[idArea].getType() == pType::Orange) {
		cout << "Time (15 30 45 1h)\n";
		cout << "Val:   1  2  3  4\n";
	}

	cin >> timeReserved;
	if (timeReserved < 1 || timeReserved > 13 && psv[idArea].getType() == pType::Blue) { cout << "Invalid time input!\n"; getch(); return; }
	if (timeReserved < 1 || timeReserved > 9 && psv[idArea].getType() == pType::Purple) { cout << "Invalid time input!\n"; getch(); return; }
	if (timeReserved < 1 || timeReserved > 4 && psv[idArea].getType() == pType::Orange) { cout << "Invalid time input!\n"; getch(); return; }

	timeReserved = convertIndToMin(timeReserved);

	totalPrice = psv[idArea].getPriceForNow();

	totalPrice = convertMinToCoins(timeReserved, totalPrice);

	idSpot -= 1;

	psv[idArea].takeSpot(idSpot, timeReserved);
	psv.addIncome(totalPrice);
	psv[idArea].infoSpot(idSpot);
	cout << "Total: " << totalPrice << " coins\n";
	getch();
}

void notificationMenu(ParkingSystem& psv) {
	const char* menu[2] = { "Extend parking time", "Exit" };
	int poz = 0;
	int menuSize = 1;

	while (true) {
		system("cls");
		psv.printNotifications();

		if (psv.getNotificationsNr() == 0) {
			getch();
			return;
		}

		cout << "\n";

		for (int i = 0; i <= menuSize; i++) {
			if (i == poz) {
				cout << "\033[0;31m" << menu[i] << "\033[0m\n";
			}
			else
				cout << menu[i] << "\n";
		}

		int t = getch();

		while (kbhit() || t) {
			if (t == 13 && poz == 0) {
				int ans, timeExtended, totalPrice;

				cout << "NrNo: ";
				cin >> ans;

				if (ans < 1 || ans > psv.getNotificationsNr()) {
					cout << "Invalid Notification Number!\n";
					return;
				}

				ps nt = psv.getNotification(--ans);

				if (psv[nt.parkingAreaId].getType() == pType::Blue) {
					cout << "Time (15  30  45  1h  1:15h  1:30h  2h  2:30h  3h  4h  6h  12h  24h)\n";
					cout << "Val:   1   2   3   4    5      6     7    8     9  10  11   12   13\n";
				}

				if (psv[nt.parkingAreaId].getType() == pType::Purple) {
					cout << "Time (15 30 45 1h 1:15h 1:30h 2h 2:30h 3h)\n";
					cout << "Val:  1   2  3  4   5     6    7   8    9\n";
				}

				if (psv[nt.parkingAreaId].getType() == pType::Orange) {
					cout << "Time (15 30 45 1h)\n";
					cout << "Val:   1  2  3  4\n";
				}
				
				cin >> timeExtended;
				if (timeExtended < 1 || timeExtended > 13 && psv[nt.parkingAreaId].getType() == pType::Blue) { cout << "Invalid time input!\n"; getch(); return; }
				if (timeExtended < 1 || timeExtended > 9 && psv[nt.parkingAreaId].getType() == pType::Purple) { cout << "Invalid time input!\n"; getch(); return; }
				if (timeExtended < 1 || timeExtended > 4 && psv[nt.parkingAreaId].getType() == pType::Orange) { cout << "Invalid time input!\n"; getch(); return; }

				timeExtended = convertIndToMin(timeExtended);
				totalPrice = psv[nt.parkingAreaId].getPriceForNow();
				totalPrice = convertMinToCoins(timeExtended, totalPrice);

				psv.extendSpotTime(nt, timeExtended);

				cout << "Time extended for spot id " << nt.id + 1 << " from parking area " << nt.parkingAreaId << " with " << timeExtended << " mins\n";
				cout << "Total: " << totalPrice << " coins\n";

				getch();
				return;
			}
			else if (t == 13 && poz == 1) {
				return;
			}
			else {
				int key = getch();
				if (key == 72) {
					poz--;
					if (poz < 0)
						poz = menuSize;
					break;
				}
				if (key == 80) {
					poz++;
					if (poz > menuSize)
						poz = 0;
					break;
				}
			}
		}
	}
}

void menu(ParkingSystem &psv) {
	const char* m[3] = { "Reserve a place", "Notifications", "Exit" };

	int poz = 0;
	int menuSize = 2;

	while (true) {
		system("cls");
		psv.refreshNotification();
		psv.removeOldNotification();
		psv.printParkingAreas();
		cout << "\nTotal: " << psv.getIncome() << " coins\n\n";

		//print menu
		for (int i = 0; i <= menuSize; i++) {
			if (i == poz && i != menuSize - 1) {
				cout << "\033[0;31m" << m[i] << "\033[0m\n";
			}
			else if (i == poz && i == menuSize - 1) {
				cout << "\033[0;31m" << m[i] << '(' << psv.getNotificationsNr() << ')' << "\033[0m\n";
			}
			else {
				if (i == menuSize - 1) {
					cout << m[i] << '(' << psv.getNotificationsNr() << ')' << "\n";
				}
				else
					cout << m[i] << "\n";
			}
		}

		//get input
		int t = getch();

		while (kbhit() || t) {
			if (t == 13) {
				switch (poz) {
				case 0: reserveSpotMenu(psv); break;
				case 1: notificationMenu(psv); break;
				case 2: return;
				}
				break;
			}
			else {
				int key = getch();
				if (key == 72) {
					poz--;
					if (poz < 0)
						poz = menuSize;
					break;
				}
				if (key == 80) {
					poz++;
					if (poz > menuSize)
						poz = 0;
					break;
				}
			}
		}
	}

	return;
}

int main() {
	vector<vector<bool>> shape1 = { {0,0,0,0,1,1,0},
									{0,1,1,0,1,1,0},
									{0,1,1,0,1,1,0},
									{0,1,1,0,1,1,0},
									{0,1,1,0,0,0,0} };

	vector<vector<bool>> shape2 = { {1,1,1,1,1,1},
									{0,0,0,0,0,0},
									{1,1,1,1,1,0},
									{1,1,1,1,1,0},
									{0,0,0,0,0,0},
									{1,1,1,1,1,0} };

	vector<vector<bool>> shape3 = { {1,0,0,1},
									{1,0,0,1},
									{1,0,0,1},
									{1,0,0,1},
									{1,0,0,1},
									{1,0,0,1}, };

	ParkingSystem psv;
	psv.addParkingArea(shape1, 400, pType::Blue, 16, 15);
	psv.addParkingArea(shape2, 401, pType::Purple, 21, 35.5);
	psv.addParkingArea(shape3, 402, pType::Orange, 12, 80);

	menu(psv);
}