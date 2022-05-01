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

## Building & Running
* Step 1: Build all project (appliccation and tests), than run (respectively for x32 or x64 version):<br>
`cmake -G "Visual Studio 16 2019" -A Win32 -S \path_to_source\ -B "build32"`<br>
`cmake -G "Visual Studio 16 2019" -A x64 -S \path_to_source\ -B "build64"`<br>
`cmake --build build32 --config Release`<br>
`cmake --build build64 --config Release`<br>

* Step 2: copy files `.\bin\requests.json`, `.\bin\config.json`, `.\bin\answers.json` and `.\bin\resources` folder from `.\bin` folder to the `.\bin\Release` folder.
* Step 3: run the application or tests:
  * To run application type:<br>
`.\bin\Release\Search_in_files`<br>
  * To run tests type:<br>
`.\bin\Release\Google_Tests_run`<br>

## Files specification
* config.json<br>
File where specified name and version of application.<br>
Here you can also change the maximal quantity of relevant pages that will be put into answers.json (max_respones).<br>
Default content:<br>
```json
{
    "config": 
    {
        "name": "FileSearchEngine",
        "version": "0.1",
        "max_responses": 5
    },
    "files": 
    [
        "resources/file001.txt",
        "resources/file002.txt"
    ]
}
```

* requests.json<br>
File where specified requests for search.<br>
Each separate request sholud be entered after ',' on new line. Example below.<br>
Example content:<br>
```json
{
    "requests":
    [
        "tiger fox",
        "wolf bird",
        "monkey"
    ]
}
```
* answers.json<br>
File where search result will be written in JSON format.<br>
Example content:<br>
```json
{
  "request0": {
    "relevance": [
      {
        "docid": 1,
        "rank": 1.0
      },
      {
        "docid": 0,
        "rank": 0.6700000166893005
      }
    ],
    "result": true
  },
  "request1": {
    "relevance": [
      {
        "docid": 0,
        "rank": 1.0
      },
      {
        "docid": 1,
        "rank": 1.0
      }
    ],
    "result": true
  },
  "request2": {
    "relevance": [],
    "result": false
  }
}
```
<i>NOTE:<br>docid - identificator of a relevant document,<br>rank - relative index,<br>result - true (if relevance is found), or false (if document absolutely not relevant for certain request).</i>
