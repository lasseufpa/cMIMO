import os
from diff_pdf_visually import pdfdiff

# Caminho para os arquivos PDF de entrada
pdf1_path = 'home/marco/Documentos/versãoatual.pdf'
pdf2_path = 'home/marco/Downloads/SBRT_2024-MIMOSystemDevelopment_ACDIOApproachtoTelecommunicationsStudy.pdf'

# Caminho para o arquivo PDF de saída
output_diff_path = 'home/marco/Imagens/diff.pdf'

# Comparar os PDFs e gerar o arquivo de diferenças
pdfdiff(pdf1_path, pdf2_path, output_diff_path)

print(f"As diferenças foram salvas em: {output_diff_path}")
