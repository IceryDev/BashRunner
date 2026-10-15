# Bash Runner
A KRunner plugin that allows passing parameters to custom bash scripts.

### Usage
#### Initialisation
Bash Runner creates a folder to search for bash scripts. 
To open this folder, open KRunner using your shortcut (Default: Alt + Space), type the prefix for Bash Runner (Default: "/sh") plus the directory command (So that it becomes "/sh -d") and confirm. 
Place any bash scripts (must end with ".sh") you want to run into that folder, and the runner will now recognise your file.

#### Script Format
There are two header types you can add to your bash script to make using this runner easier; however, they are optional to use:
- **The script has no parameters:** Place `#noparam` after `#!/bin/bash`.
- **The script has parameters:** Place `#params: <param1> <param2> ...` instead.

#### Running Scripts
<p align="center">
<img width="684" height="86" alt="runner" src="https://github.com/user-attachments/assets/44980b56-8b8a-4434-bcfd-32b3e6406d8e" />
</p>

Search for your bash script using the runner's prefix followed by a space and then your query:
```krunner
/sh myExampleFile.sh
```
Then confirm. Bash Runner will do one of the following:
- If a script is marked as `#noparam`, it will be executed directly.
- If not, it will prompt again with the parameter template provided in the target script if it exists; if not, it will place a placeholder stating there is no parameter specified in the file.
The headers act as a guidance tool and do not affect the capability of the runner. Meaning you can still pass three arguments where two specified.
```krunner
/sh myExampleFile.sh =<param1> <param2>
```
In this case, replace the parameters with your arguments and confirm.

**Tip:** You are not required to use angled brackets in this step; feel free to place any frequently used parameters there so that the runner autocompletes to those.

_**If the script does not run with the runner:**_ Check its execute permissions; if there aren't any, add them with `chmod +x /path/`

#### Argument Syntax
You don't have to surround each argument with anything, but in this case, the spaces will act as a delimiter. If you want to include a space in a parameter, surround it with double quotes. 

If you want to include quotes or any escaped characters, surround them again with double quotes and use a backslash before them.

Examples:
```krunner
/sh myExampleFile.sh =arg1 "arg with space" "arg with \"escaped character\""
```
##### Parameter Invalidation
Since there is no simple way of providing a separate KRunner bar for this plugin, both search and parameter steps use the same interface.
To avoid collision between this, I have implemented parameter invalidation, which refers to the behaviour of the runner that returns to the search step when it detects the user is no longer trying to enter parameters.

Therefore, the parameter step will fall back to the search step if the user modifies any of the non-parameter part of the query.

##### Changing Runner Priority
While this part is optional, if you want Bash Runner results to rank first in your KRunner queries, you can do this through System Settings/Plasma Search:
<p align="center">
<img width="400" height="290" alt="priority" src="https://github.com/user-attachments/assets/606041e7-f66f-4499-ba8a-8ba7d97c9d55" />
</p>

### Configuration
To configure the runner prefix or the target file, go to System Settings/Plasma Search and click on the gear icon near Bash Runner:
<p align="center">
<img width="685" height="494" alt="config" src="https://github.com/user-attachments/assets/8ec7fd2e-e6cf-4def-9050-3d24f4d11985" />
</p>

### Installation
Clone the repository into a folder:
```bash
git clone https://github.com/IceryDev/BashRunner.git
```
Make sure you are in the BashRunner directory and run:
```bash
bash install.sh
```

### Dependencies
This plugin requires KDE Plasma 6 and, at minimum, Qt 6.5.0. Other than these, the installer script will prompt you to install any other missing dependencies.

#### Tested Distros & KDE Version
✅ **CachyOS:**
- Kernel: 7.1.3-2-cachyos
- KDE Plasma Version: 6.7.2
- Qt Version: 6.11.1

*Note: This plugin should also work for Debian-based distributions; however, I have not tested the build code. Feel free to open an issue if there are any problems.*
