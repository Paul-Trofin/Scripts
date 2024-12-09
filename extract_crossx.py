from bs4 import BeautifulSoup

# Open and read the HTML file
with open('crossx.html', 'r') as file:
    html_content = file.read()

# Parse the HTML content using BeautifulSoup
soup = BeautifulSoup(html_content, 'html.parser')

# Find all rows that contain run data
rows = soup.find_all('tr')

# List to store (cm_energy, cross_section, error) tuples
data = []

# Loop through each row and extract the energy and cross section
for row in rows:
    columns = row.find_all('td')
    if len(columns) >= 5:  # Check if the row has enough columns to contain the data
        energy_text = columns[1].text.strip()  # Energy
        cross_section_text = columns[3].text.strip()  # Cross section

        # Check if energy_text contains the pattern "x" for splitting
        if "x" in energy_text and "GeV" in energy_text:
            # Extract the two energy values by removing "e- e+" and any extra spaces
            energy_text_cleaned = energy_text.replace("e- e+", "").strip()

            # Now split the cleaned text to get the two energy values
            energy_values = energy_text_cleaned.split('x')
            try:
                energy_1 = float(energy_values[0].replace("GeV", "").strip())
                energy_2 = float(energy_values[1].replace("GeV", "").strip())
                
                # Sum the energies to get the center-of-mass energy
                cm_energy = energy_1 + energy_2

                # Split cross_section_text into cross_section and error
                if "±" in cross_section_text:
                    cross_section_parts = cross_section_text.split("±")
                    cross_section = float(cross_section_parts[0].strip())
                    error = float(cross_section_parts[1].strip())
                else:
                    cross_section = float(cross_section_text.strip())
                    error = 0.0  # Default to 0 if no error is present

                # Append the data as a tuple (cm_energy, cross_section, error) to the list
                data.append((cm_energy, cross_section, error))
            except ValueError:
                print(f"Skipping invalid energy or cross section values: {energy_values}, {cross_section_text}")
        else:
            print(f"Skipping row with unexpected energy format: {energy_text}")

# Sort the data by cm_energy (the first element of each tuple)
data.sort(key=lambda x: x[0])

# Open the output file to write the sorted data
with open('crossx_energy.txt', 'w') as output_file:
    output_file.write("Energy (GeV)\tCross Section (pb)\tError (pb)\n")
    
    # Write each sorted (cm_energy, cross_section, error) tuple to the file
    written_energies = set()
    for cm_energy, cross_section, error in data:
        if (cm_energy < 75 or cm_energy > 105) and cm_energy < 200 and cm_energy not in written_energies:
            output_file.write(f"{cm_energy}\t{cross_section}\t{error}\n")
            print(f"Extracted {cm_energy}")
            written_energies.add(cm_energy)


print("Data extraction and sorting completed. Check the 'crossx_energy.txt' file.")
