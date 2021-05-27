#include "ParkingSystemClass.h"

ParkingSpot::ParkingSpot(int id, int paId) {
	this->id = id;
	this->parkingAreaId = paId;
	this->inUse = false;
	this->endParkingTime = 0;
}

ParkingArea::ParkingArea(std::vector<std::vector<bool>> shape, int id, pType type, int capacity, float price) : parkingAreaId(id) {
	this->shape = shape;
	this->type = type;
	this->capacity = capacity;
	this->price = price;
	this->psInUse = 0;

	for (int i = 0; i < capacity; i++) {
		parkingSpots.push_back(ps(i, id));
	}
}

ps ParkingArea::operator [] (int i) { return parkingSpots[i]; }

std::vector<std::vector<bool>> ParkingArea::getShape() const { return shape; }
int ParkingArea::getId() const { return parkingAreaId; }
pType ParkingArea::getType() const { return type; }
int ParkingArea::getCapacity() const { return capacity; }
float ParkingArea::getInitialPrice() const { return price; }
int ParkingArea::getPsInUse() const { return psInUse; }

void ParkingArea::takeSpot(int id, int t) {
	parkingSpots[id].inUse = true;
	parkingSpots[id].endParkingTime = time(0) + (t * 60);
	psInUse++;
}

void ParkingArea::releaseSpot(int id) {
	parkingSpots[id].endParkingTime = 0;
	parkingSpots[id].inUse = false;
	psInUse--;
}

void ParkingArea::infoSpot(int id) {
	tm* t = localtime(&parkingSpots[id].endParkingTime);

	if (parkingSpots[id].inUse)
		std::cout << "\nSpot " << id + 1 << " is reserved until " << t->tm_year + 1900 << "." << t->tm_mon << "." << t->tm_mday << " at " << t->tm_hour << ":" << t->tm_min << "!\n";
	else
		std::cout << "\nSpot " << id + 1 << " is unreserved!\n";
}

void ParkingArea::extendSpotTime(int id, int timeExtended) {
	timeExtended *= 60;
	parkingSpots[id].endParkingTime += timeExtended;
}

float ParkingArea::getPriceForNow() {
	time_t now = time(0);
	tm* t = localtime(&now);

	//from 8:00 PM to 8:00 AM price is 0
	if (type == pType::Blue) {
		if (t->tm_hour >= 20 && t->tm_hour < 8)
			return 0.f;
		return price;
	}

	//from 12:00 PM to 6:00 PM price increase with 1.5
	if (type == pType::Purple) {
		if (t->tm_hour >= 12 && t->tm_hour < 18)
			return price * 1.5f;
		return price;
	}

	//from 2:00 PM to 8:00 PM price is double
	if (type == pType::Orange) {
		if (t->tm_hour >= 14 && t->tm_hour < 20)
			return price * 2;
		return price;
	}

	return price;
}

ParkingSystem::ParkingSystem() {
	this->income = 0.f;
};

ParkingArea& ParkingSystem::operator [] (int id) {
	for (unsigned int i = 0; i < PA.size(); i++) {
		if (PA[i].getId() == id) return PA[i];
	}

	throw - 1;
}

void ParkingSystem::addParkingArea(std::vector<std::vector<bool>> shape, int id, pType type, int capacity, float price) {
	PA.push_back(ParkingArea::ParkingArea(shape, id, type, capacity, price));
}

void ParkingSystem::addIncome(float price) { income += price; }

float ParkingSystem::getIncome() const { return income; }
int ParkingSystem::getNotificationsNr() { return notifications.size(); }
ps ParkingSystem::getNotification(int ind) { return notifications[ind]; }

void ParkingSystem::extendSpotTime(ps spot, int timeExtended) {
	for (unsigned int i = 0; i < PA.size(); i++) {
		if (PA[i].getId() == spot.parkingAreaId)
			PA[i].extendSpotTime(spot.id, timeExtended);
	}

	for (unsigned int i = 0; i < notifications.size(); i++) {
		if (notifications[i].parkingAreaId == spot.parkingAreaId && notifications[i].id == spot.id)
			notifications.erase(notifications.begin() + i);
	}
}

void ParkingSystem::refreshNotification() {
	time_t now = time(0);

	for (ParkingArea& p : PA) {
		int size = p.getCapacity();
		for (int i = 0; i < size; i++) {
			if (p[i].inUse) {
				time_t end = p[i].endParkingTime;

				//verify if it exists in notification to do not add it multiple times
				bool found = false;
				for (unsigned int j = 0; j < notifications.size(); j++) {
					if (notifications[j].endParkingTime == end) found = true;
				}

				//add to notifications if remaining time is less than 10 min
				if (!found && end - now <= 600 && end - now > 0) {
					notifications.push_back(p[i]);
				}

				if (end - now < 0) {
					p.releaseSpot(i);
				}
			}
		}
	}
}

void ParkingSystem::removeOldNotification() {
	time_t now = time(0);

	for (unsigned int i = 0; i < notifications.size(); i++) {
		time_t end = notifications[i].endParkingTime;
		if (end - now < 0)
			notifications.erase(notifications.begin() + i);
	}
}

void ParkingSystem::printNotifications() {
	if (notifications.empty()) {
		std::cout << "There are no notifications!\n";
	}
	time_t now = time(0);

	for (unsigned int i = 0; i < notifications.size(); i++) {
		time_t end = notifications[i].endParkingTime;
		time_t min = (end - now) / 60;
		std::cout << i + 1 << ".Spot with id " << notifications[i].id + 1 << " from parking area " << notifications[i].parkingAreaId << " ends in " << min << " min!\n";
	}
}

void ParkingSystem::printParkingAreas() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (ParkingArea& p : PA) {
		std::cout << "ParkingID: " << p.getId() << "   Price\\h: " << p.getPriceForNow() << " coins\n";
		std::vector<std::vector<bool>> shp = p.getShape();
		int ind = 0;
		for (unsigned int i = 0; i < shp.size(); i++) {
			for (unsigned int j = 0; j < shp[0].size(); j++) {
				int color;
				if (shp[i][j]) {
					color = p[ind].inUse ? 64 : 32; // red or green
					SetConsoleTextAttribute(hConsole, color);
					if (ind < 9)
						std::cout << " 0" << ind++ + 1 << " ";
					else
						std::cout << " " << ind++ + 1 << " ";
					SetConsoleTextAttribute(hConsole, 7);
					std::cout << " ";
				}
				else {
					color = 117; // gray
					SetConsoleTextAttribute(hConsole, color);
					std::cout << "    ";
					SetConsoleTextAttribute(hConsole, 7);
					std::cout << " ";
				}
			}
			std::cout << "\n\n";
		}
	}
}