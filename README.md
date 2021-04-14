# system-service
This is a skeleton of a system service implementation.

The service is a daemon application which:
1. It parses a text configuration file in INI format stored in */etc/example/daemon.conf*.
2. Upon receiving signal SIGUSR1 it prints out parsed configuration to system log.
3. Upon receiving signal SIGHUP it reloads the configuration by reparsing the configuration file.
4. It provides D-Bus API that supports both operations above.

This project provides a Makefile with the following options:
```Makefile
make all
   - Compile the code and generates an executable file in ./bin folder.
```
```Makefile
make clean
   - Remove the ./bin and ./obj folders recursively.
```
```Makefile
make install
   - Place the executable file in /usr/local/bin/.
   - Place the system-service.service file in /etc/systemd/system/.
   - Place the daemon.conf (config file for the application) file in /etc/example/ (if the example directory does not exist it is created).
   - Place the com.redhat.SystemService.conf (config file for the D-Bus policies) file in /etc/dbus-1/sytem.d/.
```
```Makefile
make uninstall
   - Remove all files and directories created by make install.
```
