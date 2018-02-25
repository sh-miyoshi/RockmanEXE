def grep_with_line(file_name,keys)
	result = Array.new
	File.readlines(file_name).each_with_index do |line,index|
		keys.each do |key|
			if line.include?(key) then
				result.push(index.to_s+": "+line)
			end
		end
	end
	return result
end

file_path = "../RockmanEXE/source"
num = 0

File.open("todo.txt","w") do |fp|
	Dir.glob(file_path+"/*") do |f|
		result = grep_with_line(f,["todo","doNext","debug"])
		if !result.empty? then
			fp.puts File.basename(f)
			result.each do |v|
				fp.puts v
			end
			fp.puts ""
			num += result.length
		end
	end
end

puts "未実装項目数: "+num.to_s