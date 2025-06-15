import re

def remove_style_attributes(text):
    """
    Remove style attributes from img tags while preserving src and alt attributes.
    
    Args:
        text (str): The input text containing HTML img tags with style attributes
        
    Returns:
        str: Text with style attributes removed from img tags
    """
    # Pattern to match style="..." in img tags
    pattern = r'(<img\s+[^>]*?)\s+style="[^"]*"([^>]*>)'
    
    # Remove the style attribute but keep everything else
    cleaned_text = re.sub(pattern, r'\1\2', text, flags=re.IGNORECASE)
    
    return cleaned_text

def process_file(input_file, output_file=None):
    """
    Process a file to remove style attributes from img tags.
    
    Args:
        input_file (str): Path to input file
        output_file (str): Path to output file (optional)
    """
    try:
        # Read the input file
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Remove style attributes
        cleaned_content = remove_style_attributes(content)
        
        # Determine output filename
        if output_file is None:
            base_name = input_file.rsplit('.', 1)[0]
            output_file = f"{base_name}_no_style.md"
        
        # Write the cleaned content
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(cleaned_content)
        
        print(f"Successfully processed '{input_file}'")
        print(f"Cleaned content saved to '{output_file}'")
        
    except FileNotFoundError:
        print(f"Error: File '{input_file}' not found.")
    except Exception as e:
        print(f"Error processing file: {e}")

# Example usage
if __name__ == "__main__":
    # Process the Parts-List-cleaned.md file
    process_file('Parts-List-cleaned.md', 'Parts-List-github-ready.md')
    
    # Test with sample text
    sample = '<img src="image-27.png" alt="MiniBoost 5V" style="width:700px; max-width:100%;">'
    cleaned = remove_style_attributes(sample)
    print("\nSample transformation:")
    print("Before:", sample)
    print("After: ", cleaned)