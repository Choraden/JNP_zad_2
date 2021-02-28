Biblioteka standardowa języka C++ udostępnia bardzo przydatne kontenery
(np. unordered_map i unordered_set), których nie ma w bibliotece C. Często też
potrzebujemy łączyć kod w C++ z kodem w C.

Celem tego zadania jest napisanie w C++ modułu encstrset obsługującego zbiory
zaszyfrowanych ciągów znaków, tak aby można ich było używać w C. Moduł ten
(pliki encstrset.h i encstrset.cc) powinien udostępniać następujące funkcje:

unsigned long encstrset_new();

      Tworzy nowy zbiór i zwraca jego identyfikator.

void encstrset_delete(unsigned long id);

      Jeżeli istnieje zbiór o identyfikatorze id, usuwa go, a w przeciwnym
      przypadku nie robi nic.

size_t encstrset_size(unsigned long id);

      Jeżeli istnieje zbiór o identyfikatorze id, zwraca liczbę jego elementów,
      a w przeciwnym przypadku zwraca 0.

bool encstrset_insert(unsigned long id, const char* value, const char* key);

      Jeżeli istnieje zbiór o identyfikatorze id i element value po
      zaszyfrowaniu kluczem key nie należy do tego zbioru, to dodaje ten
      zaszyfrowany element do zbioru, a w przeciwnym przypadku nie robi nic.
      Szyfrowanie jest symetryczne, za pomocą operacji bitowej XOR. Gdy klucz
      key jest krótszy od value, to należy go cyklicznie powtórzyć. Wynikiem
      jest true, gdy element został dodany, a false w przeciwnym przypadku.

bool encstrset_remove(unsigned long id, const char* value, const char* key);

      Jeżeli istnieje zbiór o identyfikatorze id i element value zaszyfrowany
      kluczem key należy do tego zbioru, to usuwa element ze zbioru, a w
      przeciwnym przypadku nie robi nic. Wynikiem jest true, gdy element został
      usunięty, a false w przeciwnym przypadku.

bool encstrset_test(unsigned long id, const char* value, const char* key);

      Jeżeli istnieje zbiór o identyfikatorze id i element value zaszyfrowany
      kluczem key należy do tego zbioru, to zwraca true, a w przeciwnym
      przypadku zwraca false.

void encstrset_clear(unsigned long id);

      Jeżeli istnieje zbiór o identyfikatorze id, usuwa wszystkie jego elementy,
      a w przeciwnym przypadku nie robi nic.

void encstrset_copy(unsigned long src_id, unsigned long dst_id);

    Jeżeli istnieją zbiory o identyfikatorach src_id oraz dst_id, to kopiuje
    zawartość zbioru o identyfikatorze src_id do zbioru o identyfikatorze
    dst_id, a w przeciwnym przypadku nic nie robi.

Należy ukryć przed światem zewnętrznym wszystkie zmienne globalne i funkcje
pomocnicze nie należące do wyspecyfikowanego interfejsu modułu.

Moduł encstrset powinien wypisywać na standardowy strumień błędów informacje
diagnostyczne. Moduł ten może sprawdzać poprawność wykonania funkcji za pomocą
asercji. Kompilowanie z parametrem -DNDEBUG powinno wyłączać wypisywanie
i asercje. Obsługa standardowego wyjścia diagnostycznego powinna być realizowana
z użyciem strumienia C++ (tzn. iostream).

Parametr value o wartości NULL jest niepoprawny. Z kolei wartość NULL parametru
key lub pusty napis key oznaczają brak szyfrowania.

Oczekiwane rozwiązanie powinno korzystać z kontenerów i metod udostępnianych
przez standardową bibliotekę C++. Nie należy definiować własnych struktur lub
klas. W szczególności nie należy przechowywać przekazanych przez użytkownika
wskaźników const char* bezpośrednio, bowiem użytkownik może po wykonaniu
operacji modyfikować dane pod uprzednio przekazanym wskaźnikiem lub zwolnić
pamięć.

W rozwiązaniu nie należy nadużywać kompilacji warunkowej. Fragmenty tekstu
źródłowego realizujące wyspecyfikowane operacje na zbiorach nie powinny zależeć
od sposobu kompilowania – parametr -DNDEBUG lub jego brak (inaczej wersja
diagnostyczna nie miałaby sensu).

Przykład użycia znajduje się w pliku encstrset_test1.c. Przykład informacji
diagnostycznych wypisywanych przez powyższy przykład użycia znajduje się
w pliku encstrset_test1.err.

Aby umożliwić używanie modułu encstrset w języku C++, przy kompilowaniu pliku
nagłówkowego encstrset.h w C++ interfejsy modułu encstrset mają znaleźć się
w przestrzeni nazw jnp1. Przykład użycia znajduje się w pliku
encstrset_test2.cc. Przykład informacji diagnostycznych wypisywanych przez
powyższy przykład użycia znajduje się w pliku encstrset_test2.err.

Przykłady można skompilować za pomocą poleceń:

g++ -Wall -Wextra -O2 -std=c++17 -c encstrset.cc -o encstrset.o
gcc -Wall -Wextra -O2 -std=c11 -c encstrset_test1.c -o encstrset_test1.o
g++ -Wall -Wextra -O2 -std=c++17 -c encstrset_test2.cc -o encstrset_test2.o
g++ encstrset_test1.o encstrset.o -o encstrset_test1
g++ encstrset_test2.o encstrset.o -o encstrset_test2

Rozwiązanie powinno zawierać pliki encstrset.h i encstrset.cc, które należy
umieścić w repozytorium w katalogu
