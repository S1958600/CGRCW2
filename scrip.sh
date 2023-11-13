for file in *.cpp; do
    base_name=$(basename "$file" .cpp)
    header_file="${base_name}.h"
    if [ ! -f "$header_file" ]; then
        echo "#ifndef ${base_name}_h" >> "$header_file"
        echo "#define ${base_name}_h" >> "$header_file"
        echo "// Add your declarations here" >> "$header_file"
        echo "#endif" >> "$header_file"
    fi
done