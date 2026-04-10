# Prosty Silnik 3D: OpenGL + SDL2

Ten projekt to podstawowa aplikacja graficzna napisana w C++, wykorzystująca **OpenGL 4.1**, **SDL2** do zarządzania oknem i wejściem, oraz bibliotekę **GLM** do operacji matematycznych w przestrzeni 3D. Aplikacja renderuje obracający się, kolorowy kwadrat i pozwala na swobodne poruszanie się po scenie za pomocą prostej kamery pierwszoosobowej (FPS).

## 🚀 Funkcje
* **Renderowanie OpenGL:** Używa nowoczesnego potoku graficznego (Core Profile) z wykorzystaniem VAO, VBO i EBO.
* **Kamera 3D:** Pełna obsługa macierzy widoku i projekcji (Model-View-Projection).
* **Nawigacja po scenie:** Poruszanie się w przestrzeni za pomocą klawiatury oraz obracanie kamery za pomocą myszy (Mouse Look).
* **Zewnętrzne shadery:** Ładowanie i kompilacja kodu shaderów (Vertex & Fragment) bezpośrednio z plików zewnętrznych.

## 🛠 Wymagania i Zależności
Aby skompilować i uruchomić projekt, potrzebujesz następujących bibliotek:
* **C++ Compiler** (wspierający standard C++11 lub nowszy)
* **[SDL2](https://www.libsdl.org/)** - Obsługa okien, kontekstu OpenGL i wejścia (klawiatura/mysz).
* **[GLAD](https://glad.dav1d.de/)** - Ładowanie rozszerzeń i funkcji OpenGL.
* **[GLM](https://github.com/g-truc/glm)** - Biblioteka matematyczna dla grafiki 3D (nagłówki).

## 🎮 Sterowanie
Aplikacja przechwytuje kursor myszy zaraz po uruchomieniu, pozwalając na płynne rozglądanie się.

* **Mysz:** Rozglądanie się / Obrót kamery (lewo/prawo)
* **Strzałka w górę (Up):** Ruch do przodu
* **Strzałka w dół (Down):** Ruch do tyłu
* **Strzałka w lewo (Left):** Ruch w lewo (strafing)
* **Strzałka w prawo (Right):** Ruch w prawo (strafing)
* **Zamknięcie okna:** Kończy działanie programu (Escape nie jest domyślnie podpięty, można zamknąć okno systemowo).

## 📂 Oczekiwana struktura plików
Zwróć uwagę, że kod ładuje shadery z konkretnej ścieżki względnej. Aby program działał poprawnie bez modyfikacji kodu, upewnij się, że struktura Twojego projektu wygląda mniej więcej tak:

```text
├── src/
│   ├── shaders/
│   │   ├── vert.glsl    # Vertex shader
│   │   └── frag.glsl    # Fragment shader
│   ├── main.cpp         # Główny plik z pętlą gry i renderowaniem
│   └── CAMERA.HPP       # Plik nagłówkowy klasy kamery
│   └── CAMERA.CPP       # Implementacja klasy kamery (jeśli wydzielona)
├── build/               # Folder z plikiem wykonywalnym (np. po użyciu CMake)
