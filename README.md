![# Search-in-files](search-in-files.png)<br>
![GitHub author](https://img.shields.io/badge/made_by-OldScripter-009919)
![GitHub repo size](https://img.shields.io/github/repo-size/OldScripter/Search-in-files)
![GitHub author](https://img.shields.io/badge/made_on-C++-red)
<br><i> Search for words in files</i> | <i> Поиск слов в файлах.</i>

## Description | Описание
![GitHub author](https://img.shields.io/badge/-ENG-blue)<br>
Application for searching of request words (JSON file requests.json) in resources text files (path to resources files are specified in config.json). Search result is putting into answers.json.<br><br>
![GitHub author](https://img.shields.io/badge/-RUS-red)<br>
Приложение для поиска запрашиваемых слов (JSON файл requests.json) в текстовых файлах ресурсов (путь к ресурсам указан в config.json). Результат поиска помещается в answers.json.
## Technologies
![GitHub author](https://img.shields.io/badge/C++-14-005199?style=for-the-badge)<br>

![GitHub author](https://img.shields.io/badge/CMake-3.22-005199?style=for-the-badge)<br>
https://cmake.org/<br>

![GitHub author](https://img.shields.io/badge/JSON-3.10.5-orange?style=for-the-badge)<br>
https://github.com/nlohmann/json<br>

## Building
Before building clean the outputfolder:<br>
`cmake --build .\cmake-build-debug --target clean -- -j 6`<br>

If you would like to build all project (appliccation and tests), than run:<br>
`cmake --build .\cmake-build-debug --target all -- -j 6`<br>
To run application type:<br>
`.\bin\Search_in_files`<br>
To run tests type:<br>
`.\bin\Google_Tests_run`<br>

If you would like to build only application:<br>
`cmake --build .\cmake-build-debug --target Search_in_files -- -j 6`<br>
To run application type:<br>
`.\bin\Search_in_files`<br>

## Running
When the building is finished Just run from 
