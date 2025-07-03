#ifndef MAP_H
#define MAP_H

#include <string>

const int MAX_SIZE = 100;  // Maksimum il sayısı (projenin gereksinimlerine göre ayarlayabilirsiniz)

class Map {

public:
    int distanceMatrix[MAX_SIZE][MAX_SIZE]; // 2D mesafe matrisi
    bool visited[MAX_SIZE];  // İl ziyaret durumlarını takip eder

    Map(); // Haritayı başlatan yapıcı

    // Mesafe verilerini bir dosyadan yükler ve mesafe matrisini doldurur
    void loadDistanceData(const std::string& filename);

    // İki il arasındaki mesafenin belirtilen maxDistance aralığında olup olmadığını kontrol eder
    bool isWithinRange(int provinceA, int provinceB, int maxDistance) const;

    // Bir ili ziyaret edilmiş olarak işaretler
    void markAsVisited(int province);

    // Bir ilin zaten ziyaret edilip edilmediğini kontrol eder
    bool isVisited(int province) const;

    // Tüm illeri tekrar ziyaret edilmemiş olarak işaretler
    void resetVisited();

    // Ziyaret edilen illerin sayısını döner
    int countVisitedProvinces() const;

    // İki il arasındaki mesafeyi döner
    int getDistance(int provinceA, int provinceB) const;
};

#endif // MAP_H
