import requests
import json

# from delphi.GrFN.networks import GroundedFunctionNetwork


def main():
    webservice = "http://localhost:9000"
    res = requests.post(
        "%s/pdf_to_mentions" % webservice,
        headers={"Content-type": "application/json"},
        json={
            "pdf": "/Users/phein/repos/DARPA-ASKE/delphi/scripts/program_analysis/2005-THE ASCE STANDARDIZED REFERENCE EVAPOTRANSPIRATION EQUATION.pdf"
        },
    )

    json_dict = res.json()
    json.dump(json_dict, open("ASCE-mentions.json", "w"))


if __name__ == "__main__":
    main()
