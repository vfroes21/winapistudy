# winapistudy
The goal of this repository is to document (and share) my study of WinAPI.
</br></br></br>
# About
All the codes (and the project itself) were written using Visual Studio 2022. The codebase is made up of multiple projects under the same solution (.sln), each one containings its `main.c` file and `main` functions.
</br></br></br>
# Executing
In order to execute the codes of this repository, you must:
</br></br>
1. Clone this project
2. Open the project in Visual Studio, by double clicking the `.sln` file located at the root folder of the repository
3. Right click the solution at the Solution Manager menu (the top-level node representing your solution, not the individual projects) and select `Properties` in the submenu</br></br>
   ![Captura de tela 2024-05-10 164620](https://github.com/vfroes21/winapistudy/assets/71036803/2b1b581c-c9ff-4f37-88f0-0d954582d40a)
   </br></br>
4. Under `Common Properties`, select the section labeled `Startup Project`
5. Select which project you want to execute and click `OK` to save changes
</br></br></br>
# Projects
<h3>helloworld</h3>
A simple Hello World program that outputs a wide character string. In this example we print the russian word for "compiler".
</br></br>
<h3>systemfunctions</h3>
A program that outputs basic info and statistics of the computer, like its name, current user, memory usage, CPU, and system paths.
