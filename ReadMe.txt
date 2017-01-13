

Fisierul executabil se gaseste in folderul Release
Pentru o functionare corecta a programului, folderul Resources (din folderul Release) trebuie sa se afle in acelasi folder cu fisierul executabil (Quoridor.exe)
Controale: 2 Jucatori: Jucatorul Albastru :W,A,S,D
		       Jucatorul Portocaliu : Sageti
	   4 Jucatori: Sageti





==== SETARILE PENTRU A COMPILA FOLOSIND LIBRARIA GRAFICA SFML ====


Visual Studio:

1.Click pe File > New > Project... (Ctrl+Shift+N).
2.Din noua fereastra deschisa, selectati Win32 Console Application (Se poate gasi la Visual C++ > Win32).
3.In Win32 Application Wizard apasati pe butonul Finish.

4.In tab-ul Project selectati <Numele aplicatiei > Properties...
5.In fereastra deschisa selectati C/C++ > General
In campul Additionl Include Directiories tastati locatia librariei grafice SFML\include (Exemplu: C:\Proiect\SFML\include) Click Apply
6.Acum selectati C/C++ > Preprocessor
In camplul Preprocessor Deffinitions apasati pe sageata, dupa <Edit...> 
In noua fereastra deschisa in prima caseta scrieti

SFML_STATIC
GLEW_STATIC
UNICODE

Click pe Ok, dupa Apply
7.Selectati C/C++ > Code Generation
In campul Runtime Library selectati Multi-threaded DLL (/MD) dupa click pe Apply
8.Selectati Linker > General
In campul Additional Library Directories tastati locatia librariei grafice SFML\lib (Exemplu: C:\Proiect\SFML\lib). Click Apply
9.Selectati Linker > Input
In campul Additional Dependencies apasati pe sageata, dupa <Edit...> 
In noua fereastra deschisa in prima caseta scrieti

sfml-audio-s.lib
sfml-graphics-s.lib
sfml-network-s.lib
sfml-window-s.lib
sfml-system-s.lib
openal32.lib
jpeg.lib
freetype.lib
ws2_32.lib
gdi32.lib
opengl32.lib
winmm.lib
flac.lib
vorbisenc.lib
vorbisfile.lib
vorbis.lib
ogg.lib

Click pe Ok, dupa Apply

10.Selectati Linker > System
In campul SubSystem selectati Console (/SUBSYSTEM:CONSOLE). Click Apply
11.Selectati Linker > Advanced
In campul Target Machine selectati dupa preferinta ( de preferat MachineX86 (/MACHINE:X86) )
Click Ok

12.Daca ati selectat ( sau era selectat MachineX86 (/MACHINE:X86) ) in fereastra proiectului (nu a proprietatilor) click pe Build > Configuration Manager
La Active solution platform asigurati-va ca este selectat x86.

13. Pentru a folosi libraria grafica SFML adaugati #include <SFML/Graphics.hpp>
