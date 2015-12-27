0.upto(32767).each do |n|
	val = `cd ..;echo "memory #{n}\nexit\n"|./debug`.split("\n").first.to_i

	puts "not #{n}" if n%1000 == 0
	puts n if val == 32775
end