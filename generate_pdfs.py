import os
import subprocess
import markdown
from pygments import highlight
from pygments.lexers import CppLexer, TextLexer
from pygments.formatters import HtmlFormatter

EDGE_PATH = r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"
CWD = os.getcwd()

CSS_BASE = """
@page {
    size: A4;
    margin: 15mm 15mm 15mm 15mm;
    @bottom-right {
        content: counter(page);
    }
}

body {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 10.5pt;
    line-height: 1.5;
    color: #24292e;
    background-color: #ffffff;
    margin: 0;
    padding: 0;
}

h1, h2, h3, h4, h5, h6 {
    margin-top: 18pt;
    margin-bottom: 8pt;
    font-weight: 600;
    line-height: 1.25;
    color: #1a202c;
    page-break-after: avoid;
}

h1 {
    font-size: 20pt;
    border-bottom: 2px solid #2b6cb0;
    padding-bottom: 6pt;
    color: #1a365d;
}

h2 {
    font-size: 15pt;
    border-bottom: 1px solid #e2e8f0;
    padding-bottom: 4pt;
    color: #2b6cb0;
}

h3 {
    font-size: 12pt;
    color: #2d3748;
}

p, ul, ol {
    margin-top: 0;
    margin-bottom: 8pt;
}

table {
    border-collapse: collapse;
    width: 100%;
    margin: 10pt 0;
    font-size: 9pt;
    page-break-inside: avoid;
}

table th, table td {
    padding: 5pt 7pt;
    border: 1px solid #cbd5e0;
}

table th {
    background-color: #edf2f7;
    font-weight: 600;
    text-align: left;
    color: #2d3748;
}

table tr:nth-child(even) td {
    background-color: #f7fafc;
}

code {
    font-family: "Cascadia Code", "Consolas", "Courier New", monospace;
    font-size: 8.5pt;
    background-color: #edf2f7;
    padding: 1.5pt 3pt;
    border-radius: 3pt;
    color: #c53030;
}

pre {
    font-family: "Cascadia Code", "Consolas", "Courier New", monospace;
    font-size: 8pt;
    background-color: #f8fafc;
    border: 1px solid #e2e8f0;
    border-radius: 4pt;
    padding: 8pt;
    overflow-x: auto;
    page-break-inside: avoid;
    line-height: 1.35;
}

pre code {
    background-color: transparent;
    padding: 0;
    color: #2d3748;
    font-size: 8pt;
}

blockquote {
    margin: 8pt 0;
    padding: 6pt 12pt;
    color: #4a5568;
    border-left: 4px solid #3182ce;
    background-color: #ebf8ff;
    border-radius: 0 4pt 4pt 0;
    page-break-inside: avoid;
}

.cover-page {
    text-align: center;
    padding: 40pt 20pt;
    page-break-after: always;
    border: 3px double #2b6cb0;
    border-radius: 8pt;
    margin-bottom: 20pt;
}

.cover-college {
    font-size: 16pt;
    font-weight: bold;
    color: #1a365d;
    margin-bottom: 6pt;
    text-transform: uppercase;
}

.cover-dept {
    font-size: 13pt;
    color: #2b6cb0;
    font-weight: 600;
    margin-bottom: 4pt;
}

.cover-sub {
    font-size: 11pt;
    color: #4a5568;
    margin-bottom: 20pt;
}

.cover-title {
    font-size: 22pt;
    font-weight: bold;
    color: #2c5282;
    margin: 30pt 0 10pt 0;
    line-height: 1.3;
}

.cover-meta {
    font-size: 11pt;
    color: #4a5568;
    margin-top: 40pt;
    line-height: 1.6;
}

.file-header {
    background: #2b6cb0;
    color: white;
    padding: 6pt 10pt;
    border-radius: 4pt 4pt 0 0;
    font-weight: bold;
    font-size: 11pt;
    margin-top: 20pt;
    page-break-after: avoid;
}

.source-container {
    border: 1px solid #cbd5e0;
    border-top: none;
    border-radius: 0 0 4pt 4pt;
    margin-bottom: 20pt;
    background: #ffffff;
}

.toc-box {
    background: #f7fafc;
    border: 1px solid #e2e8f0;
    border-radius: 6pt;
    padding: 12pt;
    margin-bottom: 25pt;
    page-break-inside: avoid;
}
"""

def render_pdf_with_edge(html_path, pdf_path):
    abs_html = os.path.abspath(html_path).replace("\\", "/")
    abs_pdf = os.path.abspath(pdf_path)
    cmd = [
        EDGE_PATH,
        "--headless",
        "--disable-gpu",
        "--no-pdf-header-footer",
        f"--print-to-pdf={abs_pdf}",
        f"file:///{abs_html}"
    ]
    print(f"[*] Rendering PDF via Edge: {pdf_path}...")
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode == 0 and os.path.exists(abs_pdf):
        print(f"[+] Success: {pdf_path} ({os.path.getsize(abs_pdf)} bytes)")
        return True
    else:
        print(f"[-] Error generating {pdf_path}: {res.stderr}")
        return False

# ============================================================================
# 1. GENERATE PROJECT_REPORT.pdf
# ============================================================================
def build_report_pdf():
    print("\n--- Building PROJECT_REPORT.pdf ---")
    with open("PROJECT_REPORT.md", "r", encoding="utf-8") as f:
        md_content = f.read()

    # Convert Markdown to HTML
    html_body = markdown.markdown(
        md_content,
        extensions=["tables", "fenced_code", "toc"]
    )

    full_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Smart Campus Management System - Project Report</title>
<style>
{CSS_BASE}
</style>
</head>
<body>
{html_body}
</body>
</html>
"""
    html_file = "PROJECT_REPORT.html"
    pdf_file = "PROJECT_REPORT.pdf"

    with open(html_file, "w", encoding="utf-8") as f:
        f.write(full_html)

    render_pdf_with_edge(html_file, pdf_file)

# ============================================================================
# 2. GENERATE SOURCE_CODE.pdf
# ============================================================================
def build_source_code_pdf():
    print("\n--- Building SOURCE_CODE.pdf ---")
    source_files = [
        ("main.cpp", "Master Application Coordinator & Main Entry Point"),
        ("src/Common.hpp", "Shared Student Model, ANSI UI Themes & Input Guards"),
        ("src/Module1_StudentSearch.hpp", "Module 1: Student Information Management & Searching"),
        ("src/Module2_SortingFiles.hpp", "Module 2: Student Record Sorting & External File Handling"),
        ("src/Module3_SparseMatrix.hpp", "Module 3: Sparse Matrix Representation & Triplet Grade Analysis"),
        ("src/Module4_LinkedLists.hpp", "Module 4: Dynamic Campus Services via Linked Lists (SLL, DLL, CLL)"),
        ("src/Module5_StacksQueues.hpp", "Module 5: Reversible Operations, Expression Conversion & Queues"),
        ("src/Module6_Trees.hpp", "Module 6: Hierarchical Indexing via Trees (BST, AVL, B+ Tree)"),
        ("src/Module7_CampusGraph.hpp", "Module 7: Campus Network Routing (Graph Matrix/List, Prim, Dijkstra)"),
        ("src/Module8_HashTable.hpp", "Module 8: Constant-Time Retrieval via Open Addressing Hashing"),
        ("src/Module9_Complexity.hpp", "Module 9: Master Complexity Analysis & Theoretical Performance"),
        ("grades.txt", "External Raw Input Grade Data File")
    ]

    formatter = HtmlFormatter(style="friendly", linenos=True, cssclass="highlight")
    pygments_css = formatter.get_style_defs('.highlight')

    html_parts = []
    html_parts.append(f"""
    <div class="cover-page">
        <div class="cover-college">PIMPRI CHINCHWAD COLLEGE OF ENGINEERING (PCCOE), PUNE</div>
        <div class="cover-dept">Department of Information Technology</div>
        <div class="cover-sub">S.Y. B.Tech Information Technology | Academic Year 2026–27</div>
        <hr style="border: 0; border-top: 1px solid #cbd5e0; margin: 20pt 0;">
        <div class="cover-title">SMART CAMPUS INFORMATION AND RESOURCE MANAGEMENT SYSTEM</div>
        <div style="font-size: 14pt; color: #4a5568; margin-top: 10pt; font-weight: 600;">COMPLETE SOURCE CODE COMPENDIUM</div>
        <div class="cover-meta">
            <strong>Course:</strong> Data Structures and Algorithms (Course Code: IT204)<br>
            <strong>Standard:</strong> Modern C++17 (Zero External Library Dependencies)<br>
            <strong>Architecture:</strong> Modular Multi-File Header & Controller Hierarchy<br>
            <strong>Total Implementation Modules:</strong> 9 Integrated Subsystems
        </div>
    </div>
    """)

    # Table of contents for source files
    html_parts.append("""
    <div class="toc-box">
        <h2 style="margin-top:0; border:none; color:#1a365d;">Source Code Index & File Manifest</h2>
        <table style="margin:0;">
            <thead>
                <tr>
                    <th style="width: 35%;">File Path</th>
                    <th style="width: 50%;">Subsystem / Module Description</th>
                    <th style="width: 15%;">Lines of Code</th>
                </tr>
            </thead>
            <tbody>
    """)

    for rel_path, desc in source_files:
        if os.path.exists(rel_path):
            with open(rel_path, "r", encoding="utf-8") as f:
                lines = len(f.readlines())
            anchor = rel_path.replace("/", "_").replace(".", "_")
            html_parts.append(f"""
                <tr>
                    <td><a href="#{anchor}" style="color:#2b6cb0; text-decoration:none; font-family:monospace; font-weight:bold;">{rel_path}</a></td>
                    <td>{desc}</td>
                    <td style="text-align:right;">{lines} lines</td>
                </tr>
            """)
    html_parts.append("</tbody></table></div>")

    # Syntax highlight each file
    for rel_path, desc in source_files:
        if not os.path.exists(rel_path):
            continue
        with open(rel_path, "r", encoding="utf-8") as f:
            code_text = f.read()

        lexer = CppLexer() if rel_path.endswith((".cpp", ".hpp")) else TextLexer()
        highlighted_code = highlight(code_text, lexer, formatter)

        anchor = rel_path.replace("/", "_").replace(".", "_")
        html_parts.append(f"""
        <div id="{anchor}" style="page-break-before: always;">
            <div class="file-header">
                <span>📄 {rel_path}</span>
                <span style="float: right; font-weight: normal; font-size: 9pt; opacity: 0.9;">{desc}</span>
            </div>
            <div class="source-container">
                {highlighted_code}
            </div>
        </div>
        """)

    full_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Smart Campus - Complete Source Code</title>
<style>
{CSS_BASE}
{pygments_css}
.highlight pre {{
    margin: 0;
    padding: 8pt;
    border: none;
    font-size: 7.5pt;
    line-height: 1.35;
}}
.highlight table {{
    margin: 0;
    border: none;
}}
.highlight td {{
    border: none;
    padding: 0;
}}
.linenos {{
    color: #a0aec0;
    padding-right: 8pt !important;
    text-align: right;
    user-select: none;
    border-right: 1px solid #e2e8f0 !important;
}}
</style>
</head>
<body>
{''.join(html_parts)}
</body>
</html>
"""
    html_file = "SOURCE_CODE.html"
    pdf_file = "SOURCE_CODE.pdf"

    with open(html_file, "w", encoding="utf-8") as f:
        f.write(full_html)

    render_pdf_with_edge(html_file, pdf_file)

if __name__ == "__main__":
    build_report_pdf()
    build_source_code_pdf()
    print("\n[+] Both PDF documents generated successfully!")
