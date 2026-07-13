#!/usr/bin/env python3

"""Generate txt source files for the data directory.
While they'll be tracked with version control (so building's easier for
anyone using the project), this will script still exists as an updator.

This script currently builds:
    - data/quotes.txt

The bottom of the file has the 'main' function if tracing an issue.
"""

import json
import pathlib
import requests


PROJECT_PATH = pathlib.Path(__file__).parent.parent
DATA_DIR = PROJECT_PATH / "data"


def setup():
    DATA_DIR.mkdir(exist_ok=True)


def download(url: str, filepath: str):
    filename = pathlib.Path(filepath).name
    response = requests.get(url)
    if (response.status_code == 200):
        with open(filepath, "w", encoding="utf-8") as f:
            _ = f.write(response.text)
        print(f"{filename} downloaded")
    else:
        print(f"Failed to download {filename}")


def quotes():
    quotes_url = "https://raw.githubusercontent.com/monkeytypegame/monkeytype/refs/heads/master/frontend/static/quotes/english.json"
    quotes_filepath = str(DATA_DIR / f"quotes.{quotes_url.split(".")[-1]}")
    quotes_final_file = str(DATA_DIR / "quotes.txt")

    download(quotes_url, quotes_filepath)
    data = list()
    with open(quotes_filepath, "r") as f:
        data = json.load(f)["quotes"]
        data.sort(key=lambda x: x["length"])
        #print(data[0].keys()) #text, source, length, id
        #print([x["length"] for x in data])

    data = [x for x in data if x["text"] != ""]
    with open(quotes_final_file, "w") as f:
        _ = f.write(f"all:{len(data)}\n")
        _ = f.write(f"short:{len([x for x in data if x["length"] <= 100])}\n")
        _ = f.write(f"medium:{len([x for x in data if 100 < x["length"] <= 200])}\n")
        _ = f.write(f"long:{len([x for x in data if 200 < x["length"] <= 300])}\n")
        _ = f.write(f"thicc:{len([x for x in data if x["length"] > 300])}\n")

        for quote in data:
            quote_str = repr(quote["text"]).strip("'").replace("\\'", "'")
            if ("\"" not in quote_str[1:-1]):
                quote_str = quote_str.replace("\"", "")
            _ = f.write(quote_str + "\n")
    print(f"{quotes_final_file.split("/")[-1]} built")


def main():
    setup()
    quotes()


if __name__ == "__main__":
    main()
