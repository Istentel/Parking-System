#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <Windows.h>

enum class pType { Orange, Purple, Blue };

typedef struct ParkingSpot {
	int id;
	int parkingAreaId;
	bool inUse;
	time_t endParkingTime;

	ParkingSpot(int id, int paId);
}ps;

class ParkingArea {
private:
	std::vector<ps> parkingSpots;
	std::vector<std::vector<bool>> shape;
	const int parkingAreaId;
	pType type;
	int capacity;
	float price;
	int psInUse;
public:
	ParkingArea(std::vector<std::vector<bool>> shape, int id, pType type, int capacity, float price);

	ps operator [] (int i);

	std::vector<std::vector<bool>> getShape() const;
	int getId() const;
	pType getType() const;
	int getCapacity() const;
	float getInitialPrice() const;
	int getPsInUse() const;

	void takeSpot(int id, int t);

	void releaseSpot(int id);

	void infoSpot(int id);

	void extendSpotTime(int id, int timeExtended);

	float getPriceForNow();

};

class ParkingSystem {
private:
	std::vector<ParkingArea> PA;
	std::vector<ps> notifications;
	float income;
public:
	ParkingSystem();

	ParkingArea& operator [] (int id);

	void addParkingArea(std::vector<std::vector<bool>> shape, int id, pType type, int capacity, float price);

	void addIncome(float price);
	float getIncome() const;

	int getNotificationsNr();

	ps getNotification(int ind);

	void extendSpotTime(ps spot, int timeExtended);

	void refreshNotification();

	void removeOldNotification();

	void printNotifications();

	void printParkingAreas();
};