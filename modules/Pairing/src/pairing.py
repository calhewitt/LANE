#!/usr/bin/env python2
import math, sys, os
from readBCA import *

class Cluster:
    universalClusterID = -1
    chipNo = 0 #The number of the chip (0 to 4)
    polar = 0.0
    azimuth = 0.0 #angles in radians, azimuth is measured with pole perpendicular to chip and 0 being pointing towards x being positive
    xposition = 0.0
    yposition = 0.0
    meanEnergy = 0.0
    #type = "Other"
    paired = False
    pairedWith = None
    localClusterNum = -1
    probMissed = -1.0
    probIsGamma = 0.0    

    def copy(self):
        copy = Cluster()
        copy.universalClusterID = self.universalClusterID
        copy.chipNo = self.chipNo
        copy.polar = self.polar
        copy.azimuth = self.azimuth
        copy.xposition = self.xposition
        copy.yposition = self.yposition
        copy.meanEnergy = self.meanEnergy
        copy.paired = self.paired
        copy.pairedWith = self.pairedWith
        copy.probMissed = self.probMissed
        return copy

class group:
    Q = []
    clusters = []

ChipSize = 256 #ChipSize in pixels
PixelWidth = 0.055 #PixelWidth in mm

h = 15.0 #hieght of the bottom of the chip surface of TPX 0,...,3 above the readout layer in TPX 4 (This is a complete guess. Need to get actual value)
d = 10.0 #distance from the edges of the chip surface of TXP 4 to the readout layer in TPX 0,...,3 (This is also a complete guess and may be different for diffent chip numbers)


############################ Other Functions ########################################
def getValInRange(x,mn,mx):
    #Gets x in the range mn <= x < mx by adding the difference
    d = mx - mn
    while x < mn:
        x += d
    while x >= mx:
        x -= d
    return x
def myRange(mn,mx,step):
    #A range which allows for non-integer step
    ret = []
    for i in range(int(math.floor(mn /step)), int(math.floor(mx/step))):
        ret.append(i*step)
    return ret

################################ ANGLE DISTRIBUTIONS #############################################
wrongWayRate = 0.1 #The probabilty of the program which converts the azimuth from range -math.pi/2<azimuth<=math.pi/z
#to the range -math.pi<azimuth<=math.pi
def azimuthAnglePDWillOutputGivActualValue(actualValue, outputValue, polar):
    #modeled as a Gausian
    #Since the value that is required is obtained by arctan it can be out by pi so this
    #add/subracts pi
    sd = 0.025 #arbituary
    k = 1/(sd * math.sqrt(math.pi))
    x = actualValue - outputValue
    if polar < 0.15:
        return 1/(2*math.pi)
    
    x = getValInRange(x, -math.pi, math.pi)

    #gets probability density
    x = x/sd
    rightWayPD = k * math.e ** (-x**2) * (1-wrongWayRate)
    x *= sd
    x += math.pi
    x = getValInRange(x, -math.pi, math.pi)
    x /= sd
    wrongWayPD = k * math.e ** (-x**2) * wrongWayRate
    #print "azimuth densities PD will output given actual value"
    #print actualValue, outputValue, polar
    #print rightWayPD, wrongWayPD
    return wrongWayPD + rightWayPD + 10 ** -20
#PDIntegrations = loadDict(precalcpaths['PD'])
                         
def polarAnglePDWillOutputGivActualValue(actualValue, outputValue):
    #modeled as a Gausian
    #Since the value that is required is obtained by arctan it can be out by pi so this
    #add/subracts pi
    sd = 0.05 #from plots
    k = 1/(sd * math.sqrt(math.pi))
    x = actualValue - outputValue

    #get x in range -pi/2 <= x < pi/2
    x = getValInRange(x, -math.pi/2, math.pi/2)

    #gets probability density
    x = x/sd
    ret = k * math.e ** (-x**2)
    #print "polar PD Will Output Giv Actual Value"
    #print actualValue, outputValue, ret
    return ret + 10 ** -20 #removes dividing by zero error

def polarAnglePDIsValGivOutput(value, outputValue):
    return polarAnglePDWillOutputGivActualValue(value, outputValue)

def azimuthAnglePDIsValGivOutput(val, outputValue, polar):
    #modeled as a Gausian
    if polar < 0.05:
        return 1/(2*math.pi)
    #Since the value that is required is obtained by arctan it can be out by pi so this
    #add/subracts pi
    sd = 0.05 * (1.6 - polar) #arbituary, allows for the fact that there will be more error in the azimuth angle if it is steeper
    #print sd
    k = 1/(sd * math.sqrt(math.pi))
    #print k
    x = val - outputValue
    #get x in range -pi/2 <= x < pi/2
    x = getValInRange(x, -math.pi, math.pi)
    #print x
    #gets probability density
    x = x/sd
    rightWayPD = k * math.e ** (-x**2) * (1-wrongWayRate)
    x *= sd
    x += math.pi
    x = getValInRange(x, -math.pi, math.pi)
    x /= sd
    wrongWayPD = k * math.e ** (-x**2) * wrongWayRate
    #print "azimuth densities PD is val given out"
    #print val, outputValue, polar
    #print rightWayPD, wrongWayPD
    return wrongWayPD + rightWayPD + 10 ** -20

################################# Energy Distribution ################################

def energyDensityBGivA(clusterA, clusterB):
    #Just doing a Gausian with a complete guess as to the sd. 
    #Energies in ??? whatever the units for the a,b,c,t values are per micro meter 
    #mean dE/dx (LET) is the best way
    #
    sd = 10.0 #I think this should be within an order of magnitude. It is a deliberate overestimate.
    k = 1/(sd * math.sqrt(2 * math.pi))
    x = (clusterA.meanEnergy - clusterB.meanEnergy)/sd
    return k * math.e ** (-x**2)

############################## Position Angle Densities for 2 clusters ##########################################

def positionAngleDensityOppBGivA(ea,eb,dis):
    dis /= PixelWidth #converts the distance for mm to pixels
    azimuth = 0.0
    if ea.xposition == eb.xposition:
        azimuth = math.pi/2
    else:
        azimuth = math.atan(float(ea.yposition - eb.yposition)/float(ea.xposition - eb.xposition))

    aAzimuth = azimuth
    bAzimuth = azimuth

    if ea.xposition < eb.xposition:
        aAzimuth = azimuth
        bAzimuth = azimuth + math.pi
    else:
        aAzimuth = azimuth + math.pi
        bAzimuth = azimuth

    if ea.xposition == eb.xposition:
        if ea.yposition < eb.yposition:
            aAzimuth = math.pi/2
            bAzimuth = -math.pi/2
        else:
            aAzimuth = -math.pi/2
            bAzimuth = math.pi/2

    r = math.sqrt((ea.xposition - eb.xposition) ** 2 + (ea.yposition - eb.yposition) ** 2)
    #print "running position angle PD Opp"
    #print r
    #print ea.xposition, ea.yposition
    #print eb.xposition, eb.yposition
    polar = 0.0
    if r == 0:
        polar = 0.0
    else:
        polar = math.atan(r/dis)
    #This gives unrealistic results so I probably made a mistake in the maths or something
    #posDensity = azimuthAnglePDIsValGivOutput(aAzimuth, ea.azimuth, polar) * polarAnglePDIsValGivOutput(polar, ea.polar) * math.cos(polar) ** 2 / (dis * (r+0.001))
    predictedR = math.tan(ea.polar) * dis
    sdr = 25.0
    #print aAzimuth, bAzimuth, azimuth
    posDensity = math.e ** (0.0 - ((r-predictedR)/sdr)**2) * azimuthAnglePDIsValGivOutput(aAzimuth, ea.azimuth, polar) / (sdr * math.pi ** 0.5)
    #print r, predictedR
    angleDensity = azimuthAnglePDWillOutputGivActualValue(bAzimuth, eb.azimuth, polar) * polarAnglePDWillOutputGivActualValue(polar, eb.polar)
    return posDensity * angleDensity

def positionAngleDensityAdjBGivA(ea,eb,ha,hb):
    aAzimuth = 0.0
    bAzimuth = 0.0
    if ea.xposition == eb.xposition:
        aAzimuth = -math.pi/2
        bAzimuth = -math.pi/2
    else:
        aAzimuth = math.atan((ea.yposition * PixelWidth + ha)/((ea.xposition - eb.xposition) * PixelWidth))
        bAzimuth = math.atan((eb.yposition * PixelWidth + ha)/((eb.xposition - ea.xposition) * PixelWidth))
        
    aAzimuth = getValInRange(aAzimuth, -math.pi,0.0)
    bAzimuth = getValInRange(bAzimuth, -math.pi,0.0)
    
    l = math.sqrt((ea.yposition * PixelWidth + ha)**2 + (((ea.xposition - eb.xposition) * PixelWidth) ** 2))
    aPolar = math.atan(l/(eb.yposition * PixelWidth + hb))

    l2 = math.sqrt((eb.yposition * PixelWidth + hb)**2 + (((ea.xposition - eb.xposition) * PixelWidth) ** 2))
    bPolar = math.atan(l2/(ea.yposition * PixelWidth + ha))
    
    xDensity = (ea.yposition * PixelWidth + ha) / (((ea.xposition - eb.xposition) * PixelWidth) ** 2 + (ea.yposition * PixelWidth + ha) ** 2) #just formula from write up
    xDensity = xDensity * azimuthAnglePDIsValGivOutput(ea.azimuth, aAzimuth, aPolar)
    #print aAzimuth, aPolar, bAzimuth, bPolar, ea.azimuth, ea.polar, eb.azimuth, eb.polar
    yDensityGivX = l / (l**2 + (eb.yposition * PixelWidth + hb)**2)
    yDensityGivX = yDensityGivX * polarAnglePDIsValGivOutput(aPolar, ea.polar)

    azimuthDensityGivPos = azimuthAnglePDWillOutputGivActualValue(bAzimuth, eb.azimuth, bPolar)
    polarDensityGivPos = polarAnglePDWillOutputGivActualValue(bPolar, eb.polar)

    #print xDensity, yDensityGivX, azimuthDensityGivPos, polarDensityGivPos

    return xDensity * yDensityGivX * azimuthDensityGivPos * polarDensityGivPos

def positionangleDensityBGivA(clustera, clusterb):
    #print "running position anlge density B Giv A"
    #Transform to one of 3 cases
    #The chips are aranged with TPX 0,...,3 going round after each other in the order 0,1,3,2 (?) with 0 in 4's positive x direction and TPX 4 at the bottom with the top of it facing TPX 1
    #The 3 cases
    #Case they are on the same chip:
    #print clustera.chipNo, clusterb.chipNo
    if clustera.chipNo == clusterb.chipNo:
        #print "#"
        return 0

    #Case they are on adjacent chips:
    ea = clustera.copy()
    eb = clusterb.copy() #set ea and eb to clustera and clusterb as they will be very similar
    ha = 0.0
    hb = 0.0 #ha and hb will depend on the position
    adj = False #the two chips are adjacent
    if clustera.chipNo == 4:
        ha = d
        hb = h
        adj = True 
        #cluster b is unchanged.
        if clusterb.chipNo == 2:
            eb.azimuth += math.pi
        if clusterb.chipNo == 0:
            eb.azimuth += math.pi
            ea.xposition = clustera.yposition
            ea.yposition = ChipSize - clustera.xposition
            ea.azimuth -= math.pi/2
        if clusterb.chipNo == 1:
            eb.azimuth += math.pi
            ea.xposition = ChipSize - clustera.xposition
            ea.yposition = ChipSize - clustera.yposition
            ea.azimuth += math.pi
        if clusterb.chipNo == 3:
            eb.azimuth += math.pi
            ea.xposition = ChipSize - clustera.yposition
            ea.yposition = clustera.xposition
            ea.azimuth += math.pi/2
    
    if clusterb.chipNo == 4:
        hb = d
        ha = h
        adj = True
        #Just a copy of "if clustera.chipNo == 4:" but swaping round a and b
        ea.azimuth += math.pi
        if clustera.chipNo == 0:
            eb.xposition = clusterb.yposition
            eb.yposition = ChipSize - clusterb.xposition
            eb.azimuth -= math.pi/2
        if clustera.chipNo == 1:
            eb.xposition = ChipSize - clusterb.xposition
            eb.yposition = ChipSize - clusterb.yposition
            eb.azimuth += math.pi
        if clustera.chipNo == 3:
            eb.xposition = ChipSize - clusterb.yposition
            eb.yposition = clusterb.xposition
            eb.azimuth += math.pi/2

    #Do a horrible case bash through the remaining configurations with a and b on adjacent chips
    #b is one chip anticlockwise from a (looking down and TPX 4) (in order 0,1,3,2 so can't just do if clustera.chipNo + 1 == clusterb.chipNo: 
    if clustera.chipNo == 0 and clusterb.chipNo == 1 or clustera.chipNo == 1 and clusterb.chipNo == 3 or clustera.chipNo == 3 and clusterb.chipNo == 2 or clustera.chipNo == 2 and clusterb.chipNo == 0:
        ea.azimuth -= math.pi/2
        eb.azimuth -= math.pi/2
        ea.xposition = clustera.yposition
        eb.xposition = clusterb.yposition
        ea.yposition = ChipSize - clustera.xposition
        eb.yposition = clusterb.xposition
        ha = d
        hb = d
        adj = True

    #Again swap round a and b for a is one chip anticlockwise from b
    if clusterb.chipNo == 0 and clustera.chipNo == 1 or clusterb.chipNo == 1 and clustera.chipNo == 3 or clusterb.chipNo == 3 and clustera.chipNo == 2 or clusterb.chipNo == 2 and clustera.chipNo == 0:
        ea.azimuth -= math.pi/2
        eb.azimuth -= math.pi/2
        ea.xposition = clustera.yposition
        eb.xposition = clusterb.yposition
        ea.yposition = ChipSize - clustera.xposition
        eb.yposition = clusterb.xposition
        ha = d
        hb = d
        adj = True

    if adj:
        return positionAngleDensityAdjBGivA(ea,eb,ha,hb)
    
    eb.azimuth += math.pi/2
    eb.xposition = ChipSize - eb.xposition
    return positionAngleDensityOppBGivA(ea,eb,2*d+ChipSize * PixelWidth)

######################### probMissed ###############################

def probMissed(cluster):
    #Couldn't get the linear interpolation stuff to work properly for this one and I have more urgent stuff to do
    #Plus this isn't a particularly important part of the program so I am just going to do this:
    return 0.1 #Should always be within an order of magnitude (or almost) of this

############################### Values for random particles #######################

def hitDensity():
    #Num hits per mm^2, assuming about 7 hits per chip per frame on average. Could make function of capture time etc.
    return 0.0028

def randPolarDensity(cluster):
    #Angles assumed to be random
    #factor of cos(polar): from for-shortening of chip (is that the write term?)
    #factor of sin(polar): from are on sphere reducing as
    #it gets closer to top (god I explained that badly)

    #=> PD = k * sin * cos = k * sin(2*polar)
    #integral(0,math.pi/2) of (k * sin(2*polar)) d polar = -1/2 k (cos(math.pi/2 * 2) - cos(0)) = k = 1
    #=> k = 1
    #=> PD = sin(2*polar)
    return math.sin(2 * cluster.polar) + 0.00001

def randAzimuthDensity(cluster):
    #Angles assumed to be random
    return 1 / (2 * math.pi)

def randEnergyDensity(cluster):
    #TODO get better idea - this is good enough for now
    return 0.001 #estimate

def probRand(cluster):
    return randAzimuthDensity(cluster) * randPolarDensity(cluster) * randEnergyDensity(cluster) * hitDensity()


############################ High level functions ####################################
def probBGivA(clustera, clusterb):
    return positionangleDensityBGivA(clustera, clusterb) * energyDensityBGivA(clustera, clusterb) #*typeProb

def getQValueAB(clustera, clusterb):
    i = probBGivA(clustera, clusterb) / (probRand(clusterb) * probMissed(clustera) * probMissed(clusterb))
    if i == 0:
        return -1.0
    return math.log(i)

def getQValues(clusters):
    #Gets the Q values for those clusters
    #Q = [[Q01,Q02,..,Q0(n-1)],[Q12,..Q1(n-1)],...,[Q(n-1)(n-2)],[]]
    #Qab (from write up) = Q[a][b-1], a<b
    Q = []
    n = len(clusters)
    for a in range(0,n):
        Q.append([])
        for b in range (a+1, n):
            Q[a].append(getQValueAB(clusters[a], clusters[b]))
    return Q

def readQ(Q,a,b):
    #Q = [[Q01,Q02,..,Q0(n-1)],[Q12,..Q1(n-1)],...,[Q(n-1)(n-2)]]
    #Qab (from write up) = Q[a][b-1], a<b
    #gets the Q value between a and b
    if a == b:
        #print("Error")
        return -1
    if a.localClusterNum < b.localClusterNum:
        return Q[a.localClusterNum][b.localClusterNum-a.localClusterNum-1]
    return Q[b.localClusterNum][a.localClusterNum-b.localClusterNum-1]
        
def pairClusters(Q, clusters):
    #Current algorithm:
    #Repeat Until No Change:
    #   *Remove all "misses" (ie all clusters with only negative Q values)
    #   *Remove all definite pairs
    #Split into groups
    #Go through all combinations
    n = len(clusters)
    change = True
    #Repeat Until No Change
    while change:
        #No change has yet been found
        change = False
        #Remove all "misses"
        for i in clusters:
            if not i.paired:
                hasPosQ = False #has not yet found a positive Q value for that cluster
                for j in clusters:
                    if readQ(Q,i,j) > 0:
                        hasPosQ = True
                        j = n #ends loop to save time
                if not hasPosQ:
                    i.paired = True
                    i.pairedWith = None #paired with None means missed all other chips
                    #don't need to negateAllQ as already negative
                    change = True #Marks that there has been a change

        #Remove all definite pairs
        for i in clusters:
            if not i.paired: #Skips already paired clusters                
                #Finds the highest Q value of clusters[i] and its reference and the second highest one too
                highestQ = 0.0
                secondHighestQ = 0.0
                highestj = None
                for j in clusters:
                    Qj = readQ(Q,i,j)
                    if Qj > highestQ:
                        secondHighestQ = highestQ
                        highestQ = Qj
                        highestj = j
                    elif Qj > secondHighestQ:
                        secondHighestQ = Qj

                #Finds highest Q value of clusters[highestj]
                secondHighestQj = 0.0
                for k in clusters:
                    Qk = readQ(Q,highestj,k)
                    if k != i and Qk > secondHighestQj:
                        secondHighestQj = Qk

                #checks if the Q value is sufficiently high for that pair to be definite
                if highestQ > secondHighestQ + secondHighestQj:
                    #negates the Q values so nothing is paried with them anymore
                    negateAllQ(Q,i,n)
                    negateAllQ(Q,highestj,n)
                    i.paired = True
                    i.pairedWith = highestj
                    highestj.paired = True
                    highestj.pairedWith = i
                    change = True #Marks that there has been a change
    groups = makeGroups(Q,clusters)
    pairFromGroups(groups,Q)

def makeGroups(Q,clusters):
    #Bit for splitting into groups
    groups = []
    for i in clusters:
        if not i.paired:
            currentCluster = i
            currentClustersGroup = None
            #Checks through all current groups
            for j in groups:
                for k in j:
                    if readQ(Q,i,k) > 0:
                        #A positive Q values has been found => these clusters are from the same group
                        if currentClustersGroup == None:
                            #If the current cluster has not yet been assigned a 
                            currentClustersGroup = j
                            j.append(currentCluster)
                            k = j[-1]
                        elif currentClustersGroup != j:
                            print groups
                            print j
                            currentClustersGroup += j
                            groups.remove(j)
                            k = j[-1]
            #if the current cluster does not yet have a group that it has to belong to a new one is created
            if currentClustersGroup == None:
                groups.append([currentCluster])            
    return groups

def approximatePair(group,Q):
    #Pairs the two clusters with the highest Q value between them
    highestQ = -1
    HQ1 = -1
    HQ2 = -1
    #interates over each pair
    for i in range(1, len(group)):
        for j in range(0,i):
            if readQ(Q,group[i].localClusterNum,group[j].localClusterNum) > highestQ:
                highestQ = readQ(Q,group[i].localClusterNum,group[j].localClusterNum)
                HQ1 = group[i]
                HQ2 = group[j]
    HQ1.paired = True
    HQ2.paired = True
    HQ1.pairedWith = HQ2
    HQ2.pairedWith = HQ1
    group.remove(HQ1)
    group.remove(HQ2)

def isCompleted(subGroup):
    #printPairing(subGroup)
    #Checks wether or not this is the final combination
    n = len(subGroup)
    isCompleted = True
    for i in range(0,n//2):
        if subGroup[i].pairedWith == None or subGroup.index(subGroup[i].pairedWith) != n - i - 1:
            isCompleted = False
    if n % 2 == 1 and subGroup[n//2].pairedWith != None:
        isCompleted = False
    
    return isCompleted

def nextCombo(combo):
    #gives the next combination of paired/unpaired clusters
    #An algorithm for stepping through all combinations
    n = len(combo)
    subGroup = [] #fills up the sub group from the end of the group until that section is not ended
    i = n - 1
    while isCompleted(subGroup):
        #print subGroup
        nextItem = combo[i]
        if nextItem.pairedWith == None:
            subGroup.append(nextItem)
        elif nextItem.pairedWith.localClusterNum > nextItem.localClusterNum:
            subGroup.append(nextItem)
            subGroup.append(nextItem.pairedWith)
            subGroup = mergeSortClusters(subGroup)
        i -= 1
    if subGroup[0].pairedWith == None:
        subGroup[0].pairedWith = subGroup[1]
        subGroup[1].pairedWith = subGroup[0]
    else:
        subGroup[0].pairedWith = subGroup[subGroup.index(subGroup[0].pairedWith) + 1]
        subGroup[0].pairedWith.pairedWith = subGroup[0]
    for j in range(1,len(subGroup)):
        if subGroup[0].pairedWith != subGroup[j]:
            subGroup[j].pairedWith = None
    return combo

def getQCombo(Q, combination):
    sumQ = 0.0
    for i in combination:
        if i.pairedWith != None and i.pairedWith.localClusterNum > i.localClusterNum:
            sumQ += readQ(Q, i,i.pairedWith)
    return sumQ  

def slowPair(group,Q):
    highestPairing = []
    for i in group:
        highestPairing.append(None)
    
    highestSum = 0.0
    while not isCompleted(group):
        group = nextCombo(group)
        sumQ = getQCombo(Q,group)
        if sumQ > highestSum:
            highestSum = sumQ
            highestPairing = []
            for i in group:
                highestPairing.append(i.pairedWith)

    for i in range(0,len(group)):
        group[i].pairedWith = highestPairing[i]
        group[i].paired = True

def pairFromGroups(groups,Q):
    #pairs the clusters after they have been divided into groups
    maxSizeToCheck = 7 #the maximum size that slowPair willl be run on
    for i in range(0,len(groups)):
        while len(groups[i]) > maxSizeToCheck:
            approximatePair(groups[i],Q)        
        slowPair(groups[i],Q)
            
def mergeSortClusters(clusters):
    ln = len(clusters)
    if ln == 1:
        return clusters
    else:
        splitPoint = ln // 2
        leftHalf = clusters[:splitPoint]
        rightHalf = clusters[splitPoint:]
        leftHalf = mergeSortClusters(leftHalf)
        rightHalf = mergeSortClusters(rightHalf)
        #print leftHalf,rightHalf
        clusters = []
        while len(clusters) < ln:
            if len(rightHalf) == 0 or (len(leftHalf) != 0 and leftHalf[0].localClusterNum < rightHalf[0].localClusterNum):
                clusters.append(leftHalf.pop(0))
            else:
                clusters.append(rightHalf.pop(0))
    return clusters

def negateAllQ(Q,a,n):
    for i in range(0,a.localClusterNum):
        Q[i][a.localClusterNum-i-1] = -1
    for i in range(a.localClusterNum + 1,n):
        Q[a.localClusterNum][i-a.localClusterNum-1] = -1

def getClusters(frame):
    clusters = []
    for c in frame.clusters:
        cluster = Cluster()
        cluster.chipNo = int(frame.meta['Channel'])
        cluster.azimuth = float(c['Azimuth'])
        cluster.xposition = float(c['X'])
        cluster.yposition = float(c['Y'])
        cluster.polar = float(c['Polar'])
        cluster.meanEnergy = float(c['LET'])
        if float(c['Size']) < 5:
            cluster.probIsGamma = 0.99
        else:
            cluster.probIsGamma = 0.01
        clusters.append(cluster)
    return clusters

def reorganizeFrames(frames):
    IDs = []
    clusters = {} #dictionary ID:[clusters]
    for f in frames:
        if f.meta['ID'] in IDs:
            clusters[f.meta['ID']] += getClusters(f)
        else:
            clusters[f.meta['ID']] = getClusters(f)
            IDs.append(f.meta['ID'])
    return clusters

def numberClusters(clusters):
    i = 0
    for c in clusters:
        c.localClusterNum = i
        i += 1

def printPairing(clusters):
    s = ''
    for c in clusters:
        s +='(' + str(c.localClusterNum) + ','
        if c.pairedWith == None:
            s +='None'
        else:
            s += str(c.pairedWith.localClusterNum)
        s += ') '
    print s

if __name__ == '__main__':
    if len(sys.argv) != 6:
        print "USAGE: %s input-dir output-dir masks-dir calibrations-dir configurations-dir" % (str(sys.argv[0]))
        sys.exit(1)
    
    paths = getPaths(sys.argv[2])    
    for p in paths:
        if (os.path.basename(p)).endswith('.bca'):
            print "running on", p
            frames = readFile(p)
            allClusters = reorganizeFrames(frames)
            for ID, clusters in allClusters.iteritems():
                numberClusters(clusters)            
                Q = getQValues(clusters)
                pairClusters(Q, clusters)
                #printPairing(clusters)
