import re

def remove_href_tags(text):
    """
    Remove all <a href> tags while preserving their inner content.
    
    Args:
        text (str): The input text containing HTML with <a href> tags
        
    Returns:
        str: Text with <a href> tags removed but inner content preserved
    """
    # Pattern to match <a href="..."> and </a> tags
    # This will capture everything between the opening and closing a tags
    pattern = r'<a\s+href="[^"]*"[^>]*>(.*?)</a>'
    
    # Replace the entire <a href="...">content</a> with just the content
    cleaned_text = re.sub(pattern, r'\1', text, flags=re.DOTALL | re.IGNORECASE)
    
    return cleaned_text

def process_file(input_file, output_file=None):
    """
    Process a file to remove href tags.
    
    Args:
        input_file (str): Path to input file
        output_file (str): Path to output file (optional, defaults to input_file_cleaned.txt)
    """
    try:
        # Read the input file
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Remove href tags
        cleaned_content = remove_href_tags(content)
        
        # Determine output filename
        if output_file is None:
            base_name = input_file.rsplit('.', 1)[0]
            output_file = f"{base_name}_cleaned.txt"
        
        # Write the cleaned content
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(cleaned_content)
        
        print(f"Successfully processed '{input_file}'")
        print(f"Cleaned content saved to '{output_file}'")
        
    except FileNotFoundError:
        print(f"Error: File '{input_file}' not found.")
    except Exception as e:
        print(f"Error processing file: {e}")

def process_text_directly(text):
    """
    Process text directly without file I/O.
    
    Args:
        text (str): The text to process
        
    Returns:
        str: Cleaned text
    """
    return remove_href_tags(text)

# Example usage
if __name__ == "__main__":
    # Option 1: Process your Parts-List.md file directly
    # Uncomment the line below to process your file:
    process_file('Parts-List.md', 'Parts-List-cleaned.md')
    
    # Option 2: Paste your markdown content here and process it
    # Replace the text below with your actual markdown content
    markdown_content = '''**Additional Parts**

| Part | Image | Description | Link |
| :---- | :---- | :---- | :---- |
| MiniBoost 5V | <a href="https://www.adafruit.com/product/4654"><img src="image-27.png" alt="MiniBoost 5V" style="width:700px; max-width:100%;"></a> | 3.3 to 5 Voltage Converter | [https://www.adafruit.com/product/4654](https://www.adafruit.com/product/4654) |
| Battery Coin cr1220 | <a href="https://www.digikey.com/en/products/detail/renata-batteries/RENATA-CR1220-TS-1/13283109"><img src="image-28.png" alt="Battery Coin cr1220" style="width:700px; max-width:100%;"></a> | Battery (Coin) | [https://www.digikey.com/en/products/detail/renata-batteries/RENATA-CR1220-TS-1/13283109](https://www.digikey.com/en/products/detail/renata-batteries/RENATA-CR1220-TS-1/13283109) |'''
    
    print("Processing markdown content...")
    print("="*50)
    cleaned_markdown = process_text_directly(markdown_content)
    print("CLEANED RESULT:")
    print("="*20)
    print(cleaned_markdown)
    print("\n" + "="*50)
    print("You can copy the cleaned result above and paste it back into your markdown file!")
    
    # Option 3: Save cleaned content to a new file
    with open('cleaned_markdown.md', 'w', encoding='utf-8') as f:
        f.write(cleaned_markdown)
    print("Cleaned content also saved to 'cleaned_markdown.md'")