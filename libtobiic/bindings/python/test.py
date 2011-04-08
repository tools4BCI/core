from pylibtobiic import *

classifierMI = ICClassifier("cnbi_mi", "CNBI MI Classifier", ICClassifier.ValueProb, ICClassifier.LabelBiosig)
class_rh = ICClass("0x301", 0.1)
class_lh = ICClass("0x302", 0.2)
class_bf = ICClass("0x303", 0.7)

classifierMI.AddClass(class_rh)
classifierMI.AddClass(class_lh)
classifierMI.AddClass(class_bf)

classifierERP = ICClassifier("fsl_erp", "FSL ERP Classifier", ICClassifier.ValueDist, ICClassifier.LabelCustom)
class_a = ICClass("A", 1.00)
class_b = ICClass("B", 2.00)
class_c = ICClass("C", 3.00)

classifierERP.AddClass(class_a)
classifierERP.AddClass(class_b)
classifierERP.AddClass(class_c)

message = ICMessageRapid(True)
message.AddClassifier(classifierMI)
message.AddClassifier(classifierERP)

message.Dump()

message.GetClass("cnbi_mi", "0x301").SetValue(100)
message.GetClassifier("cnbi_mi").GetClass("0x302").SetValue(200)
print message.GetClassifier("cnbi_mi").GetClass("0x302").GetValue()

buffer = ICMessage.Serialize(message)
print buffer

message2 = ICMessageRapid(True)

ICMessage.Deserialize(message2, buffer)
message2.Dump()

ICMessage.Deserialize(message2, buffer)
message2.Dump()

ICMessage.Deserialize(message2, buffer)
