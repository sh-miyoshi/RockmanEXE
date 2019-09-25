# variables
file_path = '../RockmanEXE/source'
target = %w[todo doNext debug TODO]

def grep_with_line(file_name, keys)
  result = []
  File.readlines(file_name).each_with_index do |line, index|
    keys.each do |key|
      result.push(index.to_s + ': ' + line) if line.include?(key)
    end
  end
  result
end

# main process
num = 0
File.open('todo.txt', 'w') do |fp|
  Dir.glob(file_path + '/*') do |f|
    result = grep_with_line(f, target)
    unless result.empty?
      fp.puts File.basename(f)
      result.each do |v|
        fp.puts v
      end
      fp.puts ''
      num += result.length
    end
  end
end

puts '未実装項目数: ' + num.to_s
