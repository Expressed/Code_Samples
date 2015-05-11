import os
import random
import math 

class DataMine:


  def __init__(self):
    ##Program Modifiers,
    ##DONT CHANGE UNLESS YOU KNOW WHAT YOU ARE DOING
    self.__P_WEIGHT = 1.25
    self.__N_WEIGHT = .75
    #RISK LEVEL filters
    self.__CONF_VAL_HM = .2 #Default .2
    self.__CONF_VAL_HL = .25 # Default .25
    self.__RISK_H = 100 #default 100
    self.__RISK_L = 1 #default 1
    #Criteria for saccade Filtering 
    self.__LENGTH_FIL = 150 #default 150
    self.__HEIGHT_FIL = 100 #default 100
    ##
   #set instances of the class
    self.__SCREEN_ANGLE = 38
    self.__displayList = []
    self.__xStart = 0
    self.__xEnd = 0
    self.__xList = []    
    self.__subjectList = []
    self.__conditionList = []
    self.__saccadeList = []
    self.__yValues = []
    self.__trialList = []
    self.__delayList = []
    self.__sampleList = []
    self.__yIndexes = []
    self.__xIndexes = []
    self.__trialNum = 0
    self.__sampleNum = 0
    self.__2ndSampleNum = 0
    self.__startValue = 0
    self.__endValue = 0
    self.__min = 0
    self.__max = 0
    self.__whileIndex = 0
    self.__2ndWhileIndex = 0
    self.__3rdWhileIndex = 0
    self.__sampleDifference = 0
    self.__startValue = 0
    self.__endValue = 0
    self.__finalList = []
    self.__finish = True
    self.__totalArea = 0
    self.__finalData = 'file'
    self.__totalAbsArea = 0
    self.__globalTrack1 = 0
    self.__globalTrack2 = 0

    # temporary holders for first wave of data analysis
    self.__subjectTemp = []
    self.__trialTemp = []
    self.__conditionTemp = []
    self.__cueTemp = []
    self.__latencyTemp = []
    self.__yStartTemp = []
    self.__yEndTemp = []
    self.__xEndTemp = []
    self.__xStartTemp = []
    self.__minTemp = []
    self.__maxTemp = []
    self.__yLengthTemp =[]
    self.__startTemp = []
    self.__totalAreaTemp = []
    self.__totalAbsAreaTemp = []
    self.__devDirTemp = []
    self.__deviationValue = []
    self.__devDirMaxTemp = []
    self.__devDirMinTemp = []
    self.__startTemp =[]
    self.__angleMin = []
    self.__angleMax = []

    self.__newTotalArea =0
    self.__newTotalAreaTemp = []
    self.__newAbsTotalArea = 0
    self.__newAbsTotalAreaTemp = []
    
# Accessors
  def getSaccadeList(self):
    return self.__saccadeList

  def getyValues(self):
    return self.__yValues

  def getTrialList(self):
    return self.__trialList

  def getDelayList(self):
    return self.__delayList

  def getSampleList(self):
    return self.__sampleList

  def getSubjectList(self):
    return self.__subjectList

  def getConditionList(self):
    return self.__conditionList 

  def getXList(self):
    return self.__xList

  def getDisplayList(self):
    return self.__displayList

  # Mutators
  def setFinalData(self, file):
    self.__finalData = file
    
  

#convert input txt file to corresponding list 
  def convertText(self,inputFile, inList):

    for line in inputFile:

      lineStrip = line.rstrip()
      inList.append(lineStrip)

#display list to ensure function
  def displayList(self, listIn):
    for index in listIn:
      print(index)

  def resetList(self):
    self.__yIndexes = []





# Functions det() and simul() are used to solve for system of equations.
# Borrowed from online
  def det(self, l):
      n=len(l)
      if (n>2):
          i=1
          t=0
          sum=0
          while t<=n-1:
              d={}
              t1=1
              while t1<=n-1:
                  m=0
                  d[t1]=[]
                  while m<=n-1:
                      if (m==t):
                          u=0
                      else:
                          d[t1].append(l[t1][m])
                      m+=1
                  t1+=1
              l1=[d[x] for x in d]
              sum=sum+i*(l[0][t])*(det(l1))
              i=i*(-1)
              t+=1
          return sum
      else:
          return (l[0][0]*l[1][1]-l[0][1]*l[1][0])

  def simul(self, leq,lcoeff):
      D=float(self.det(leq))
      if D==0:
          return -1
      else:
          t=0
          n=len(leq)
          l=[]
          while t<=(n-1):
              lt=[]
              for x in leq:
                  ltemp=[]
                  for y in x:
                      ltemp.append(y)
                  lt.append(ltemp)
              m=0
              while m<=(n-1):
                  lt[m][t]=lcoeff[m]
                  m+=1
              l.append(self.det(lt)/D)
              t+=1
          return l


  
  # Function will take in 6 points, X-start, Y-start, X-end, y-End, x-point, y-point, And then
  # calculate the length of the line from the x,y -points to an imaginary line calculated from the
  # xy start-end parameters
  def distance(self, Xs, Ys, Xe, Ye, Xm, Ym):
    if( Xm == '.'):
      Xm = 0
    else:
      Xm = float(Xm)
    if( Ym == '.'):
      Ym = 0
    else:
      Ym = float(Ym)
      
    #Must first do delta X seperate from the m calculation for input validation to avoid 0 division
    denom = Xs - Xe
    if(denom == 0):
      denom = 1 
    
    mSE = (Ys - Ye) / denom
    
    # input validation for Mvalue will return an unlikely large number,
    # this should probably be modified in the future
    if mSE == 0:
      mSE = 100000000
        
    bSE = Ys - (mSE * Xs)
    mMP = -1/mSE
    bMP = Ym - mMP * Xm
    
    #SImul function solves for x and y of simultaneous equations
    # returns them as a list [x,y]
    pointP = self.simul([[mSE, 1],[mMP, 1]], [bSE, bMP])
    
    
    # need to negate x value and return it 
    new = -1 * pointP.pop(0)
    pointP.insert(0, new)
    

    # calculate distance formula between predicted points and given points
    deltaX = pointP[0] - Xm
    deltaY = pointP[1] - Ym


    # distances are valued based on being positive or negative relatve to imaginary straight line. THis can help
    # to approximate the overrall saccade deviation
    
    # Abs() values can be extracted later on if +- is not needed 
    distance = (deltaX**2 + deltaY**2)**.5
    
    if Xm < pointP[0]:
      distance = (distance * -1)
    
   
    return distance 
    
#- main data mine function 
  def dataMiner(self):
  
    
    #initialize the index counter 
    index = 0
    
#- - This will run through all the data in the inputted files. It uses the saccadeList length,
    # because saccade data is the most important in terms or processing. This while loop will
    # ensure that it runs through to the end. 
    while index < len(self.__saccadeList)-1:
      
      # set the trial index to the current index 
      self.__trialIndex = index
      self.__2ndWhileIndex = 0
      oldIndex = index 
      
#- - - Second while loop will run for as long as the data is within a current trial and the
      # finish sentinel has not been set to false 
      while (self.__trialList[oldIndex] == self.__trialList[self.__trialIndex] and self.__finish == True):

        # Reset yIndexes, xIndexes, whileIndex, and finalList,  and 2nd sampleNum everytime
        # a new trial occurs 
        self.__yIndexes = []
        self.__xIndexes = []
        self.__whileIndex = 0
        self.__finalList = []
        self.__2ndSampleNum = 0
        filterVar = True 
 

#- - - - If statement grabs the sample number when the trial begins and bumps the 2ndwhileindex
         # to ensure it only happens once per trial
        if (self.__2ndWhileIndex == 0):
                  
          self.__sampleNum = self.__sampleList[index]
          self.__2ndWhileIndex += 1
         
#- - - - While loop tests for zeros in saccade list and that the index has exceeded length of data
        # file and that we are still within the same trial
        # index will bump up, and do nothing else as 0's are not worthwhile
        while ((index < len(self.__saccadeList)-1) and float(self.__saccadeList[index]) == 0 \
              and (self.__trialList[oldIndex] == self.__trialList[self.__trialIndex])):
            
            self.__trialList[self.__trialIndex]
            index += 1
            
            # note Trial index will continue to move while oldIndex remains, this can happen
            # as different indexes can return an equal value (the trial number)
            self.__trialIndex = index
            
        
#- - - - While test for 1's in saccade list and for same trial and for index hasn't exceeded length
        while (((index < len(self.__saccadeList)-1) and float((self.__saccadeList[index])) == 1 \
              and (self.__trialList[oldIndex] == self.__trialList[self.__trialIndex])\
                 and self.__subjectList[oldIndex] == self.__subjectList[self.__trialIndex])):
          self.__globalTrack2 = self.__trialList[index]
          self.__globalTrack1 = self.__subjectList[index]
        
          
#- - - - - If statement will grab the beginning  that occur after saccade has happened
          # this marks the end the saccade so it grabs the sample value 
          if (self.__whileIndex == 0):
            self.__2ndSampleNum = self.__sampleList[index]
            self.__yStartValue = self.__yValues[index-1]
            self.__xStart = self.__xList[index-1]          
            self.__whileIndex += 1
       
          if( self.__yValues[index]) == '.':
            yValueIndex = 0
          else:
            yValueIndex = float(self.__yValues[index])
          try:
            if( min(self.__yIndexes)) == '.':
              yValueMin = 0
            else:
              yValueMin = min(self.__yIndexes)
          except:
            add = 1
          
#<        # Only x and y can be added if y has not been added before. THis ensures that excess eye data isn't collected. For example, if a
            # saccade were to "loop" backwards upon itself
            # NOTE* values can only be added if the y element is less than the previous min, this approach only works for a BOTTOM to TOP
            # saccade          
          if( filterVar or float(yValueIndex) < float(yValueMin) ):
            filterVar = False 
            self.__xIndexes.append(self.__xList[index])
            self.__yIndexes.append(self.__yValues[index])
            self.__trialNum = self.__trialList[index]
          index += 1
          self.__trialIndex = index
          
#<
        # end values are no longer based upon saccade ending (in terms of 1-0 crossover) but the last points in the previously attained saccade data
        # this mean if the saccade movement doubles back the actual ending point will be at the precise moment before the saccade doubled back

        
        self.__yEndValue = self.__yValues[index]
        self.__xEnd = self.__xList[index]
        #EdgeCase1: This currently runs the program list index out of range error.
###        if(self.__subjectList[oldIndex] != self.__subjectList[self.__trialIndex]):
###          self.__yEndValue = self.__yValues[index-1]
###          self.__xEnd = self.__xList[index-1]  
 

        self.__trialIndex = index
        self.__trialList[self.__3rdWhileIndex]

#- - - - find min and max of y values
        if (float(len(self.__yIndexes)) > 0):
      

          
      #- - input validation for decimal points (noise) in data
          if( self.__xStart == '.'):
            self.__xStart = 0
          else:
            self.__xStart = float(self.__xStart)
          if( self.__yStartValue == '.'):
            self.__yStartValue = 0
          else:
            self.__yStartValue = float(self.__yStartValue)
          if( self.__xEnd == '.'):
            self.__xEnd = 0
          else:
            self.__xEnd = float(self.__xEnd)
          if( self.__yEndValue == '.'):
            self.__yEndValue = 0
          else:
            self.__yEndValue = float(self.__yEndValue)

  

          saccadeLength = ((float(self.__xEnd) - float(self.__xStart))**2 + (float(self.__yStartValue) - float(self.__yEndValue))**2)**.5
          saccadeLength2 = saccadeLength
          #EdgeCase#1: (does help other problems) saccadelength calculates abs{},if bad data inserted this may correct it
          #L2R specific 
          if(self.__yStartValue < self.__yEndValue):
            saccadeLength = (-1)*(saccadeLength)
          
          # In Bottom 2 Top experiment the Xaxis determines the min and max
          self.__min = min(self.__xIndexes)
          self.__max = max(self.__xIndexes)

          
          ##For edgeCase#1: If/else to appropriate the correct data for index vs index -1
          if(self.__subjectList[oldIndex] == self.__subjectList[self.__trialIndex]):
            # append subject
            
            self.__subjectTemp.append(str(self.__subjectList[index]))

            #append condition
            self.__conditionTemp.append(str(self.__conditionList[index]))
            
            # append cue 
            self.__cueTemp.append(str(self.__delayList[index]))
          else:
             
            self.__subjectTemp.append(str(self.__subjectList[index-1]))

            #append condition
            self.__conditionTemp.append(str(self.__conditionList[index-1]))
            
            # append cue 
            self.__cueTemp.append(str(self.__delayList[index-1]))
            
          #append trial
          self.__trialTemp.append(str(self.__trialNum))

           # append sample diference *saccade latency
          
          self.__sampleDifference = (float(self.__2ndSampleNum) - float(self.__sampleNum))
          self.__latencyTemp.append(str(self.__sampleDifference))

          # append start and end values 
          self.__yStartTemp.append(str(self.__yStartValue))
          self.__yEndTemp.append(str(self.__yEndValue))

          #print(self.__xStart, self.__xEnd)
          self.__xStartTemp.append(str(self.__xStart))
          self.__xEndTemp.append(str(self.__xEnd))

                          
          #append min and max
          self.__minTemp.append(str(self.__min))
          self.__maxTemp.append(str(self.__max))

          #append saccade length
          self.__yLengthTemp.append(str(saccadeLength))


          # Find the Index of min and max
          minIndex = self.__xIndexes.index(self.__min)
          minElementY = self.__yIndexes[minIndex]

          maxIndex = self.__xIndexes.index(self.__max)
          maxElementY = self.__yIndexes[maxIndex]
          

          #input validation
          if( maxElementY == '.'):
            maxElementY = 0
          else:
            maxElementY = float(maxElementY)
          if( minElementY == '.'):
            minElementY = 0
          else:
            minElementY = float(minElementY)

          # find the distance of the max and min deviation
          # max Distance and minDistance variable assignment have been switched due to the fact that a "mathematical" min of a curve 
          # becomes a "max" due to the fact that (0,0) is on the top left of the screen. So Y values are reversed. Thus reversing the conceptual min/max. 
 
          minDistance = self.distance(self.__xStart, self.__yStartValue, self.__xEnd, self.__yEndValue, self.__min, minElementY)
          maxDistance = self.distance(self.__xStart, self.__yStartValue, self.__xEnd, self.__yEndValue, self.__max, maxElementY)

          self.__devDirMinTemp.append((minDistance))
          self.__devDirMaxTemp.append((maxDistance))
                   
          calcIndex = 0

          self.__totalArea = 0
          self.__totalArea = 0
          self.__totalAbsArea = 0
          self.__newTotalArea = 0
          self.__newAbsTotalArea = 0

          #input validation
          if( self.__min == '.'):
            self.__min = 0
          else:
            self.__min = float(self.__min)
          if( self.__max == '.'):
            self.__max = 0
          else:
            self.__max = float(self.__min)
            
          oppMax = maxDistance
          oppMin = minDistance
          hypMax =( (float(self.__xStart) - float(self.__max) )**2 + (float(maxElementY) -  float(self.__yStartValue))**2 )**.5
          hypMin =( (float(self.__xStart) - float(self.__min) )**2 + (float(minElementY) -  float(self.__yStartValue))**2 )**.5
 
          try:
            self.__angleMin.append(str( 57.2957795*math.asin(oppMin/hypMin)))
          except:
            self.__angleMin.append(str(0))
          try:
            self.__angleMax.append(str( 57.2957795*math.asin(oppMax/hypMax)))
          except:
            self.__angleMax.append(str(0))
            
          
            
          
#- - - - -  find the summation of the area of the saccades
     
     
          while calcIndex < len(self.__xIndexes):
            #print("calcIndex", calcIndex)
            devDistance = self.distance(self.__xStart, self.__yStartValue, self.__xEnd, self.__yEndValue, self.__xIndexes[calcIndex], self.__yIndexes[calcIndex])

            if(calcIndex < len(self.__xIndexes)/2):
              devDistance = self.__P_WEIGHT*devDistance
             
            else:
              devDistance = self.__N_WEIGHT*devDistance                                                                         
            #Old Area Calculation
            self.__totalArea += devDistance
            self.__totalAbsArea += abs(devDistance)

            calcIndex += 1
            ## Try the more accurate AUtC by using rectangles ( x-length/# of saccade points is used as an approximation)
            self.__newTotalArea += (saccadeLength2/len(self.__xIndexes)*devDistance)
            self.__newAbsTotalArea += abs((saccadeLength2/len(self.__xIndexes)*devDistance))

#- - - - -                                 
          if self.__totalArea > 0:
            devDir = "Right" #+ str(self.__totalArea)
          elif self.__totalArea < 0:
            devDir = "Left" #+ str(self.__totalArea)

          else:
            devDir = "N/A"
          #self.__totalAreaTemp.append(str(self.__totalArea)) previous way of adding saccade area to a list
            # now it has become a string value of UP or DOWN
          self.__totalAreaTemp.append((self.__totalArea))
          self.__totalAbsAreaTemp.append(self.__totalAbsArea)
          self.__devDirTemp.append(devDir)
          self.__newTotalAreaTemp.append(self.__newTotalArea)
          self.__newAbsTotalAreaTemp.append(self.__newAbsTotalArea)
          
          
          # write to file
#         displayData.write("\t  ".join(self.__finalList) + '\n')
          self.__trialIndex = index
          self.__trialList[self.__trialIndex]
#- - - -         
        if (self.__trialIndex == (len(self.__saccadeList)-1)):
          self.__finish = False 
       ## add finisher mark
    print("Done")

#- This function will run and write to the final output file with the longest saccade of each trial
  def trimData(self):
    print("Trimming Data")

    self.__finalData.write(str("Subject:\tTrial:\tCondition:\tCue:\tLatency:\tStart:\tEnd:\tX-Length:\t Deviation Dir:\t AreaUnder Curve:\t \
Min\Max:\t Deviation Value(Pixels):\t Dev Value Directional:\t Dev Value Corrected:\t Dev Val Deg:\t Dev Value Dir-Deg: \t Dev Value Cor-Reg:\t SaccadeAngle:\t Abs Angle:\t Temp:\t Temp: \t AbsArea: \t Area/Dev: \t Confidence: \t RiskLevel: \n"))
    self.__finish = True
    index = 0

#- - First while loop will run through each subject within the carried over temp files from the initial data mining
    while index < len(self.__subjectTemp)-1:

      self.__finalList = []
      self.__trialIndex = index
      oldIndex = index
      yLength = 0
      finalIndex = 0

#- - - While loop will run as long as the data is on the same trial and the finish sentinel has not changed        
      while (self.__trialTemp[oldIndex] == self.__trialTemp[self.__trialIndex] and self.__finish):

#- - - - If - else decision statement will determine which saccade is longest based on y-Length
        # The if will assign the current index to finalIndex counter with the largest saccade
        # the else will bump up the current index value to keep things moving
        if float(self.__yLengthTemp[index]) > yLength:
          yLength = float(self.__yLengthTemp[index])
          finalIndex = index
          index += 1
          self.__trialIndex = index
          
        else:
          index += 1
          self.__trialIndex = index

#- - - - Sentinel decision statement to test whether the trial has finished 
        if (self.__trialIndex == (len(self.__subjectTemp)-1)):
          self.__finish = False
          index += 1
          
      # calculate the length of y-axis values from saccade         
      xDifference = abs(float(self.__xStartTemp[finalIndex]) - float(self.__xEndTemp[finalIndex]))
      
      # If trial has not finished If statement will enter and appropriate output data will be added to a list
      #print("Xlength", xLength, "yLength", yDifference)
      
#- - - X-length must be above 150 to be valid and ydifference must be less than 100 to be valid
      if yLength > self.__LENGTH_FIL and xDifference < self.__HEIGHT_FIL:
        
        #append subject
        self.__finalList.append(str(self.__subjectTemp[finalIndex]))

        #append trial
        self.__finalList.append(str(self.__trialTemp[finalIndex]))

        #append condition
        self.__finalList.append(str(self.__conditionTemp[finalIndex]))
        
        # append cue 
        self.__finalList.append(str(self.__cueTemp[finalIndex]))

        # append sample diference *saccade latency        
        self.__finalList.append(str(self.__latencyTemp[finalIndex]))

        # append start and end values 
        self.__finalList.append(str(self.__yStartTemp[finalIndex]))
        self.__finalList.append(str(self.__yEndTemp[finalIndex]))

       
        #append saccade length
        self.__finalList.append(str(self.__yLengthTemp[finalIndex]))
        
        #append Direction of curve
        self.__finalList.append(str(self.__devDirTemp[finalIndex]))

        #Append Area Under the Curve
        self.__finalList.append(str(abs(self.__newTotalAreaTemp[finalIndex])))

  #- - - - append Extremum
        if float(self.__newTotalAreaTemp[finalIndex]) < 0:
          extremum = str(self.__minTemp[finalIndex])
          devDir = self.__devDirMinTemp[finalIndex]
          angle = self.__angleMin[finalIndex]

        elif float(self.__newTotalAreaTemp[finalIndex]) > 0:
          extremum = str(self.__maxTemp[finalIndex])
          devDir = self.__devDirMaxTemp[finalIndex]
          angle = self.__angleMax[finalIndex]
                      
        else:
          extremum = str(0)
          devDir = 0
          angle = 0
        #input validation  
        if extremum == '.':
          extremum = 0
          devDir = 0                        
  

        #Sign determiner
        condition = self.__conditionTemp[finalIndex]
 
        sign = 0
        if(condition[0]=="L"):
          sign = -1
   
        if(condition[0]=="R"):
          sign = 1

        self.__finalList.append(str(extremum))

        # Append Deviation Value Abs
        self.__finalList.append(str(abs(devDir)))

        #Append the Dev Value directional
        self.__finalList.append(str(devDir))
        
        #Append Append the Dev Value Corrected
        self.__finalList.append(str(devDir*sign))
        
        #Append the Deviation Degrees Abs
        self.__finalList.append(str( (abs(devDir)/self.__SCREEN_ANGLE) ))
        
        #Append the Deviation Value Directional Degrees
        self.__finalList.append(str(devDir/self.__SCREEN_ANGLE))
        
        #Append the Dev Value Correct Degrees
        self.__finalList.append(str(devDir*sign/self.__SCREEN_ANGLE))
        
        #Append Angle
        self.__finalList.append(str(angle))

        #Append Abs Angle
        self.__finalList.append(str(abs(float(angle))))

        #######Extra Column for the meantime
        self.__finalList.append(str( abs(float(self.__devDirMinTemp[finalIndex])) ))
        self.__finalList.append(str( abs(float(self.__devDirMaxTemp[finalIndex])) ))

        #Append Abs Area
        self.__finalList.append(str(self.__newAbsTotalAreaTemp[finalIndex]))

        #Append Area divided by Deviation Ratio
        try:
          areaDevRatio = abs(self.__totalAreaTemp[finalIndex])/abs(devDir)
        except:
          areaDevRatio = abs(self.__totalAreaTemp[finalIndex])/.000000001
          
        self.__finalList.append(str(areaDevRatio))

        #append aRea divided by Abs Area (aka CONFIDENCE)
        confidence = abs(self.__newTotalAreaTemp[finalIndex])/abs(self.__newAbsTotalAreaTemp[finalIndex])
        self.__finalList.append(str(confidence))
        #RISK LEVEL
        if(confidence < self.__CONF_VAL_HM):
          if(areaDevRatio < self.__RISK_L or areaDevRatio > self.__RISK_H):
            self.__finalList.append(str("3HIGH"))
          else:
            self.__finalList.append(str("2Medium"))
        elif(confidence < self.__CONF_VAL_HL):
          if(areaDevRatio < self.__RISK_L or areaDevRatio > self.__RISK_H):
            self.__finalList.append(str("3HIGH"))
          else:
            self.__finalList.append(str("1Low"))
        else:
          self.__finalList.append(str("0N/A"))
        
        ##         #Write data to final file            
        self.__finalData.write("\t  ".join(self.__finalList) + '\n')


#- - Psuedo Progress Bar         
    print("Trimming is")
    num = 0
    while num < 100:
      num += random.randint(1,9)
      if num < 100:
        print(num, "%")
      else:
        print("100%")

  
  # To String
  def __str__(self):
    return str("SaccadeList")+ str(self.__saccadeList) + str("yvalues") + str(self.__yValues) +\
           str("trialList") + str(self.__trialList) + str("delayList") + str(self.__delayList) + \
           str("sampleList") + str(self.__sampleList) + str("displayIndex") + str(self.__yIndexes)







def main():

  dataminer = DataMine()
  
  
  file = open("condition.txt", 'r')
  file2 = open("delay.txt", 'r')
  file3 = open("saccade.txt", 'r')
  file4 = open("sample.txt", 'r')
  file5 = open("subject.txt", 'r')
  file6 = open("trial.txt", 'r')
  file7 = open("xposition.txt", 'r')
  file8 = open("yposition.txt", 'r')

  output = open("outputB2T929.txt", 'w')
  
  dataminer.convertText(file, dataminer.getConditionList())
  dataminer.convertText(file2, dataminer.getDelayList())
  dataminer.convertText(file3, dataminer.getSaccadeList())
  dataminer.convertText(file4, dataminer.getSampleList())
  
  dataminer.convertText(file5, dataminer.getSubjectList())
  dataminer.convertText(file6, dataminer.getTrialList())
  dataminer.convertText(file7, dataminer.getXList())
  dataminer.convertText(file8, dataminer.getyValues())
  
  dataminer.setFinalData(output)
  dataminer.dataMiner()
  dataminer.trimData()
  
  file.close()
  file2.close()
  file3.close()
  file4.close()

  file5.close()
  file6.close()
  file7.close()
  file8.close()
  output.close()
  print("done")


main()


  

      
      

