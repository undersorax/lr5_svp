#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>

using namespace std;

struct RoutePoint
{
    double xKm;
    double yKm;
    string name;
};

class IVehicle
{
public:
    virtual bool drive(int kilometrs) = 0;
    virtual void refuel() = 0;
    virtual void printStatus() = 0;
};

class AbstractCar : public IVehicle
{
protected:
    int totalDistance;
    int fuelLevel;
    int fuelConsumption;
    int maxFuel;

public:
    AbstractCar(int fuelConsumption, int maxFuel)
        : totalDistance(0), fuelLevel(0), fuelConsumption(fuelConsumption), maxFuel(maxFuel) {}

    bool drive(int kilometers) override {
        int requiredFuel = (kilometers * fuelConsumption) / 100;
        if (fuelLevel < requiredFuel) {
            std::cout << "Недостаточно топлива для прохождения дистанции." << std::endl;
            return false;
        }
        else {
            totalDistance += kilometers;
            fuelLevel -= requiredFuel;
            return true;
        }

    }

    void refuel() override {
        fuelLevel = maxFuel;
        std::cout << "Бак заполнен" << std::endl;
    }

    void printStatus() override {
        std::cout << "Общий пробег: " << totalDistance << " км" << std::endl;
        std::cout << "Остаток топлива в баке: " << fuelLevel << " литров" << std::endl;
    }
};

class Sedan : public AbstractCar
{
public:
    Sedan()
        : AbstractCar(8, 60)
    {}
};

class SUV : public AbstractCar
{
public:
    SUV()
        : AbstractCar(10, 70)
    {}
};

class Bus : public AbstractCar
{
public:
    Bus()
        : AbstractCar(15, 150)
    {}
};

class Bicycle : public IVehicle
{
private:
    int totalDistance = 0;
public:
    int get_totalDistance()
    {
        return totalDistance;
    }

    bool drive(int kilometrs) override
    {
        totalDistance += kilometrs;
        return true;
    }

    void refuel() override {};

    void printStatus()
    {
        cout << "Общий пробег: " << get_totalDistance() << "km" << endl;
    }
};

class Route
{
private:
    vector<RoutePoint> points;
public:
    void addPoint(const RoutePoint& point)
    {
        points.push_back(point);
    }
    void run(IVehicle* vehicle) {
        if (points.size() < 2)
        {
            cout << "there is less than 2 points";
        }
        else
        {
            for (int i = 1; i < points.size(); i++)
            {
                int distance = round(sqrt(pow(points[i - 1].xKm - points[i].xKm, 2) + pow(points[i - 1].yKm - points[i].yKm, 2)));
                bool driveSuccess = vehicle->drive(distance);
                if (!driveSuccess)
                {
                    vehicle->refuel();
                    if (!vehicle->drive(distance))
                    {
                        cout << "не удалось проехать" << endl;
                        return;
                    }
                }
                vehicle->printStatus();
            }
        }
    }
};

int main(int argc, char* argv[])
{
    if (argc < 2) { cout << "no argument (path to the file)"; return 0; }

    setlocale(LC_ALL, "RUS");

    vector <Route> routes;
    
    ifstream f1;

    f1.open(argv[1]);
    if (!f1)
    {
        cout << "файл не удалось открыть" << endl;
        return 0;
    }
    Route route_temp;
    double a = 0, b = 0;
    string c;
    while (!f1.eof())
    {
        f1 >> a;
        f1 >> b;
        f1 >> c;
        route_temp.addPoint({ a, b, c });
    }
    routes.push_back(route_temp);

    IVehicle* vehicles[4] = { new Bus, new SUV, new Sedan, new Bicycle };
    for (int j = 0; j < routes.size(); j++) {
        for (int i = 0; i < 4; i++) {
            routes[j].run(vehicles[i]);
            cout << "\n\n";
        }
    }
    for (int i = 0; i < 4; i++)
    {
        delete vehicles[i];
    }
    system("pause");
    f1.close();
    return 0;
}