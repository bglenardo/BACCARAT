#! /bin/csh -f
foreach file (*.bin)
        if (-d $file) then
            echo "Skipping $file (is a directory)"
        else
            echo "Converting $file"
            ./../tools/BaccRootReader $file
        endif
end
##           Labels to jump to exit OK (done) or not OK (error)
done:
     	exit 0
error:
   	exit 1

