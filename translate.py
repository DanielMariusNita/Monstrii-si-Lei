import csv
from googletrans import Translator

def translate_csv(input_file, output_file):
    # Initialize the translator
    translator = Translator()

    # Open the input and output files
    with open(input_file, 'r', encoding='utf-8') as input_csv, open(output_file, 'w', encoding='utf-8', newline='') as output_csv:
        # Read the input CSV file
        reader = csv.reader(input_csv)

        # Create a CSV writer for the output file
        writer = csv.writer(output_csv)

        # Translate and write each row of the input CSV file to the output CSV file
        for row in reader:
            translated_row = [translator.translate(cell, dest='ro').text for cell in row]
            writer.writerow(translated_row)

# Translate my files
translate_csv('menu.csv', 'MenuRo.csv')
translate_csv('wednesdayCast.csv', 'wednesdayCastRo.csv')